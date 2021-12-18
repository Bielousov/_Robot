#include "NeuralNet.h"
#include "Dictionary.cpp"

NeuralNet::NeuralNet(
    const uint8_t inputNodes, 
    const uint8_t hiddenNodes,
    const uint8_t outputNodes,
    HardwareSerial* serial
){
    HiddenNodes = hiddenNodes;
    InputNodes = inputNodes;
    OutputNodes = outputNodes;
    Serial = serial;
    initialize();
}

/******************************************************************
 * Private methods
 ******************************************************************/

void NeuralNet::initialize() {
    // Initialize Neural Network Topology: 
    Input = new float[InputNodes];
    Hidden = new float[HiddenNodes];
    Output = new float[OutputNodes];

    HiddenWeights = new float*[InputNodes+1];
    OutputWeights = new float*[HiddenNodes+1];
    ChangeHiddenWeights = new float*[InputNodes+1];
    ChangeOutputWeights = new float*[HiddenNodes+1];

    // Initialize Hidden Layer
    for( uint8_t i = 0; i <= InputNodes; i++ ) { 
        HiddenWeights[i] = new float[HiddenNodes];
        ChangeHiddenWeights[i] = new float[HiddenNodes];

        for( uint8_t j = 0; j < HiddenNodes; j++ ) {    
            ChangeHiddenWeights[i][j] = 0.0;   
            HiddenWeights[i][j] = randomWeight();
        }
    }

    // Initialize Ouput Layers
    for( uint8_t i = 0; i <= HiddenNodes; i++ ) {
        OutputWeights[i] = new float[OutputNodes];
        ChangeOutputWeights[i] = new float[HiddenNodes];

        for( uint8_t j = 0; j < OutputNodes; j ++ ) {    
            ChangeOutputWeights[i][j] = 0.0;     
            OutputWeights[i][j] = randomWeight();
        }
    }

    Serial->println(dictionary.get(STR_START));
}

void NeuralNet::runTrainCycle() {
    if (TrainingCycle >= MaxTrainCycles) {
        stopTraining();
        return;
    }

    TrainingCycle++;
    Error = 0.0;
    bool showReport = TrainingCycle % ReportFrequencey == 0;
    
    // Randomize order of training patterns
    for( p = 0; p < TrainDataSize; p++) {
        q = random(TrainDataSize);
        r = RandomizedIndex[p];
        RandomizedIndex[p] = RandomizedIndex[q];
        RandomizedIndex[q] = r;
    }
    
    // Cycle through each training pattern in the randomized order
    for( q = 0; q < TrainDataSize; q++ ) {    
        p = RandomizedIndex[q];
        getActivation(TrainData[p], showReport);
        backPropagation();
    }
}

/******************************************************************
* Set data to input layer
******************************************************************/
void  NeuralNet::setInputData (float* data) {
    Input = data;
}

/******************************************************************
* Compute hidden layer activations
******************************************************************/
void  NeuralNet::getHiddenLayerActivation() {
    for( uint8_t i = 0 ; i < HiddenNodes ; i++ ) {    
      Accum = HiddenWeights[InputNodes][i] ;
      for( uint8_t j = 0 ; j < InputNodes ; j++ ) {
        Accum += Input[j] * HiddenWeights[j][i] ;
      }
      Hidden[i] = activationFunction(Accum);
    }
}

float NeuralNet::activationFunction(float value) {
    return 1.0/(1.0 + exp(-value));
}

/******************************************************************
* Compute output layer activations and calculate errors
******************************************************************/
void  NeuralNet::getOutputLayerActivation() {
    for( uint8_t i = 0 ; i < OutputNodes ; i++ ) {    
        Accum = OutputWeights[HiddenNodes][i] ;
        for( uint8_t j = 0 ; j < HiddenNodes ; j++ ) {
            Accum += Hidden[j] * OutputWeights[j][i] ;
        }
        Output[i] = activationFunction(Accum);

        if (IsTraining) {
            OutputDelta[i] = (TargetData[p][i] - Output[i]) * Output[i] * (1.0 - Output[i]);
            Error += 0.5 * (TargetData[p][i] - Output[i]) * (TargetData[p][i] - Output[i]);
        }
    }
}

void  NeuralNet::backPropagation() {
    // Backpropagate errors to hidden layer
    for( uint8_t i = 0; i < HiddenNodes; i++ ) {    
        Accum = 0.0;
        
        for( uint8_t j = 0; j < OutputNodes; j++ ) {
            Accum += OutputWeights[i][j] * OutputDelta[j];
        }
        
        HiddenDelta[i] = Accum * Hidden[i] * (1.0 - Hidden[i]);
    }

    // Update Inner-->Hidden Weights
    for( uint8_t i = 0; i < HiddenNodes; i++ ) {     
        ChangeHiddenWeights[InputNodes][i] = LearningRate * HiddenDelta[i] + Momentum * ChangeHiddenWeights[InputNodes][i];
        HiddenWeights[InputNodes][i] += ChangeHiddenWeights[InputNodes][i];
    
        for( uint8_t j = 0; j < InputNodes; j++ ) { 
            ChangeHiddenWeights[j][i] = LearningRate * TrainData[p][j] * HiddenDelta[i] + Momentum * ChangeHiddenWeights[j][i];
            HiddenWeights[j][i] += ChangeHiddenWeights[j][i];
        }
    }

    // Update Hidden-->Output Weights
    for( uint8_t i = 0; i < OutputNodes; i ++ ) {    
        ChangeOutputWeights[HiddenNodes][i] = LearningRate * OutputDelta[i] + Momentum * ChangeOutputWeights[HiddenNodes][i];
        OutputWeights[HiddenNodes][i] += ChangeOutputWeights[HiddenNodes][i];
    
        for( uint8_t j = 0; j < HiddenNodes; j++ ) {
            ChangeOutputWeights[j][i] = LearningRate * Hidden[j] * OutputDelta[i] + Momentum * ChangeOutputWeights[j][i];
            OutputWeights[j][i] += ChangeOutputWeights[j][i];
        }
    }
}

void NeuralNet::printData(float* data, uint8_t size) {
    Serial->print ("[");
    for( uint8_t i = 0 ; i < size ; i++ ) {
        Serial->print (" ");
        Serial->print (data[i], 4);
        Serial->print (" ");
    }
    Serial->print ("]");
}

void NeuralNet::printInputData() {
    Serial->println ();
    Serial->print (dictionary.get(STR_INPUT));
    printData(Input, InputNodes);
};

void NeuralNet::printOutputActivation() {
    Serial->println ();
    Serial->print (dictionary.get(STR_OUTPUT));
    printData(Output, OutputNodes);
    Serial->println ();
};

void NeuralNet::printTrainSummary() {
    Serial->println ();
    Serial->println (dictionary.get(STR_SEPARATOR));
    Serial->print (dictionary.get(STR_EPOCH));
    Serial->print (TrainingCycle);
    Serial->print (" | ");
    Serial->print (dictionary.get(STR_ERROR));
    Serial->print (Error, 5);
};

float* NeuralNet::loadTrainingData(const float* data, uint8_t size, uint8_t index) {
    DataBuffer = new float[size];
    for ( uint8_t i = 0; i < size; i++) {
        DataBuffer[i] = pgm_read_float_near(&data[index * size + i]);
    }
    return DataBuffer;
}

float NeuralNet::randomWeight() {
    return 2 * (float(random(100))/100 - 0.5) * InitialWeightMax;
}

/******************************************************************
 * Public methods
 ******************************************************************/

float*  NeuralNet::getActivation(float* data) {
    setInputData(data);
    getHiddenLayerActivation();
    getOutputLayerActivation();
    return Output;
}

float*  NeuralNet::getActivation(float* data, bool debug) {
    getActivation(data);

    // Logging
    if (debug) {
        printTrainSummary();
        printInputData();
        printOutputActivation();
    }
    
    return Output;
}

void NeuralNet::test(
    const float* testData,
    const uint8_t testDataSize
) {
    Serial->println();
    Serial->println();
    Serial->println(dictionary.get(STR_RESULT));

    for ( uint8_t i = 0; i < testDataSize; i++ ) {
        getActivation(loadTrainingData(testData, InputNodes, i), true);
    }
}

void NeuralNet::train(
    const float* trainData,
    const float* targetData,
    const uint8_t trainDataSize
) {
    Error = 0.0;
    TrainingCycle = 0;
    TrainDataSize = trainDataSize;
    TrainData = new float* [TrainDataSize];
    TargetData = new float* [TrainDataSize];

    HiddenDelta = new float[HiddenNodes];
    OutputDelta = new float[OutputNodes];

    for (uint8_t i = 0; i < TrainDataSize; i++) {
        TrainData[i] = loadTrainingData(trainData,  InputNodes, i);
        TargetData[i] = loadTrainingData(targetData, OutputNodes, i);
    }

    RandomizedIndex = new uint8_t[TrainDataSize];
    for( p = 0; p < TrainDataSize; p++ ) {    
        RandomizedIndex[p] = p;
    }

    startTraining();
}

void NeuralNet::startTraining() {
    IsTraining = true;

    Serial->println (dictionary.get(STR_TRAINING_START));

    while (IsTraining) {
        runTrainCycle();

        if (Success > Error) {
            stopTraining();
        }
    }
};

void NeuralNet::stopTraining() {
    IsTraining = false;
};