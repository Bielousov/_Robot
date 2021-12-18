#ifndef NeuralNet_h
#define NeuralNet_h

#include <Arduino.h>
#include <Dictionary.h>
#include <avr/pgmspace.h>
#include <math.h>

class NeuralNet {
  public:
    NeuralNet(
      const uint8_t inputNodes, 
      const uint8_t hiddenNodes,
      const uint8_t outputNodes,
      HardwareSerial *Serial
    );

    // Config
    float InitialWeightMax = 0.5;
    float LearningRate = 0.3;
    float Momentum = 0.9;
    uint32_t MaxTrainCycles = 4294967295;
    int ReportFrequencey = 1000;
    float Success = 0.01;

    // Training control
    void startTraining();
    void stopTraining();

    void runTrainCycle();

    void train(
      const float   *trainData,
      const float   *targetData,
      const uint8_t trainDataSize
    );

    void test(
      const float   *testData,
      const uint8_t testDataSize
    );

    // Processing
    float* getActivation(float *data);
    float* getActivation(float *data, bool debug);

  
  private: 
    // Topology
    uint8_t HiddenNodes;
    uint8_t InputNodes;
    uint8_t OutputNodes;

    // Weights and biases
    float *Input;
    float *Hidden;
    float *Output;
    float **HiddenWeights;        // float HiddenWeights[InputNodes+1][HiddenNodes]
    float **OutputWeights;        // float OutputWeights[HiddenNodes+1][OutputNodes]

    // Training Data
    float   *DataBuffer;
    float   **TargetData;         // float TargetData[TrainDataSize][OutputNodes]
    float   **TrainData;          // float TrainData[TrainDataSize][InputNodes]
    uint8_t TrainDataSize;

    // Training
    float Accum;
    float Error = 0.0;
    bool IsTraining = false;
    float **ChangeHiddenWeights;  // float ChangeHiddenWeights[InputNodes+1][HiddenNodes]
    float **ChangeOutputWeights;  // float ChangeOutputWeights[HiddenNodes+1][OutputNodes]
    float *HiddenDelta;
    float *OutputDelta;
    uint8_t *RandomizedIndex;
    uint32_t TrainingCycle = 0;

    // Reusable variables
    uint8_t p, q, r;

    // Logging interface
    HardwareSerial *Serial;
    Dictionary dictionary;

    // Methods
    // -------
    void initialize();
    
    void backPropagation();
    void trainResults();

    // Layers activation
    float activationFunction(float inputValue);
    void getHiddenLayerActivation();
    void getOutputLayerActivation();
    void setInputData(float* data);

    // Debugging
    void printData(float *data, uint8_t size);
    void printInputData();
    void printOutputActivation();
    void printTrainSummary();

    // Helper functions
    float* loadTrainingData(
      const float* data,
      uint8_t size,
      uint8_t index
    );
    float randomWeight();
};

#endif