#ifndef NeuralNet_h
#define NeuralNet_h

#include <Arduino.h>
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
    int ReportFrequencey = 500;
    float Success = 0.001;

    void train(
      const float *trainData,
      const float   *targetData,
      const uint8_t patternCount
    );

    float* getActivation(float *data);

    // Topology
    float HiddenNodes;
    float InputNodes;
    float OutputNodes;

    // Weights and biases
    float *Input;
    float *Hidden;
    float *Output;
    float **HiddenWeights;        // float HiddenWeights[InputNodes+1][HiddenNodes]
    float **OutputWeights;       
  
  private: // float OutputWeights[HiddenNodes+1][OutputNodes]
    float **ChangeHiddenWeights;  // float ChangeHiddenWeights[InputNodes+1][HiddenNodes]
    float **ChangeOutputWeights;  // float ChangeOutputWeights[HiddenNodes+1][OutputNodes]

    // Training Data
    float **TrainData;          // uint8_t TrainData[PatternCount][InputNodes]
    float   **TargetData;             // float TargetData[PatternCount][OutputNodes]
    uint8_t PatternCount;

    // Training
    float Accum;
    float Error;
    bool IsTraining = false;
    float *HiddenDelta;
    float *OutputDelta;
    uint8_t *RandomizedIndex;
    uint32_t TrainingCycle;

    // Logging interface
    HardwareSerial *Serial;

    // Reusable variables
    uint8_t p, q, r;

    // Methods
    void initializeLayers();
    
    void trainCycle();
    void trainReport();
    void trainResults();
    void backPropagation();

    // Layers activation
    void inputData(uint8_t* data);
    void getHiddenLayerActivation();
    void getOutputLayerActivation();
    float activationFunction(float inputValue);

    void printInputData();
    void printOutputActivation();

    // Helper functions
    float randomWeight();
};

#endif