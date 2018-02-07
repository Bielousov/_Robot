#include "NeuralNet.h"

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

    initializeLayers();
    
    Serial->println("Neural Network Initialized.");
}

/******************************************************************
 * Public methods
 ******************************************************************/

void NeuralNet::train(
    const uint8_t *trainData,
    const float *targetData,
    const uint8_t patternCount,
    const float success
) {
    Error = 0.0;
    Success = success;
    PatternCount = patternCount;
    TrainData = new uint8_t * [PatternCount];
    TargetData = new float * [PatternCount];

    HiddenDelta = new float[HiddenNodes];
    OutputDelta = new float[OutputNodes];

    Serial->print("Beginning training…");

    for (i = 0; i < PatternCount; i++) {
        TrainData[i] = new uint8_t[InputNodes];
        TargetData[i] = new float[OutputNodes];
    
        for (j = 0; j < InputNodes; j++) {
            TrainData[i][j] = pgm_read_byte_near(&trainData[i * InputNodes + j]);
        }

        for (j = 0; j < OutputNodes; j++) {
            TargetData[i][j] = pgm_read_float_near(&targetData[i * OutputNodes + j]);
        }
    }

    RandomizedIndex = new uint8_t[PatternCount];
    for( p = 0; p < PatternCount; p++ ) {    
        RandomizedIndex[p] = p;
    }

    Debug = false;
    IsTraining = true;
    TrainingCycle = 0;
    
    while(IsTraining) {
        trainCycle();
        trainReport();

        if (TrainingCycle == MaxTrainCycles || Success > Error) {
            IsTraining = false;
        }
    }

    Debug = true;
}

float*  NeuralNet::getActivation(uint8_t *data) {
    inputData(data);

    getHiddenLayerActivation();
    getOutputLayerActivation();

    // Logging
    if (Debug) {
        printInputData();
        printOutputActivation();
    }
    
    return Output;
}


/******************************************************************
 * Private methods
 ******************************************************************/

void NeuralNet::initializeLayers() {
    // Initialize Layer Connections: 
    Input = new uint8_t[InputNodes];
    Hidden = new float[HiddenNodes];
    Output = new float[OutputNodes];

    HiddenWeights = new float*[InputNodes+1];
    OutputWeights = new float*[HiddenNodes+1];
    ChangeHiddenWeights = new float*[InputNodes+1];
    ChangeOutputWeights = new float*[HiddenNodes+1];

    // Initialize Hidden Layer
    for( i = 0; i <= InputNodes; i++ ) { 
        HiddenWeights[i] = new float[HiddenNodes];
        ChangeHiddenWeights[i] = new float[HiddenNodes];

        for( j = 0; j < HiddenNodes; j++ ) {    
            ChangeHiddenWeights[i][j] = 0.0;   
            HiddenWeights[i][j] = 2.0 * ( randomWeight() - 0.5 ) * InitialWeightMax;
        }
    }

    // Initialize Ouput Layers
    for( i = 0; i <= HiddenNodes; i++ ) {
        OutputWeights[i] = new float[OutputNodes];
        ChangeOutputWeights[i] = new float[HiddenNodes];

        for( j = 0; j < OutputNodes; j ++ ) {    
            ChangeOutputWeights[i][j] = 0.0;     
            OutputWeights[i][j] = 2.0 * ( randomWeight() - 0.5 ) * InitialWeightMax;
        }
    }
}

void NeuralNet::trainCycle() {
    TrainingCycle++;
    Error = 0.0;
    Debug = false;
    
    // Randomize order of training patterns
    for( p = 0; p < PatternCount; p++) {
        q = random(PatternCount);
        r = RandomizedIndex[p];
        RandomizedIndex[p] = RandomizedIndex[q];
        RandomizedIndex[q] = r;
    }
    
    // Cycle through each training pattern in the randomized order
    for( q = 0; q < PatternCount; q++ ) {    
        p = RandomizedIndex[q];
        getActivation(TrainData[p]);
        backPropagation();
    }
}

void NeuralNet::trainReport() {
    if (TrainingCycle == 1 || TrainingCycle % ReportFrequencey == 0) {
        Debug = true;
        Serial->println(); 
        Serial->println(); 
        Serial->print ("TrainingCycle: ");
        Serial->print (TrainingCycle);
        Serial->print ("  Error = ");
        Serial->println (Error, 5);

        for( p = 0 ; p < PatternCount ; p++ ) { 
            Serial->println();
            Serial->print ("  Training Pattern: ");
            Serial->println (p);      
            Serial->print ("  Train Data (input) ");
            for( i = 0 ; i < InputNodes ; i++ ) {
            Serial->print (TrainData[p][i], DEC);
            Serial->print (" ");
            }
            Serial->print ("  Target Data (output) ");
            for( i = 0 ; i < OutputNodes ; i++ ) {
            Serial->print (TargetData[p][i], 2);
            Serial->print (" ");
            }

            getActivation(TrainData[p]);
        }
    }
}

/******************************************************************
* Set data to input layer
******************************************************************/
void  NeuralNet::inputData (uint8_t *data) {
    Input = data;
}

/******************************************************************
* Compute hidden layer activations
******************************************************************/
void  NeuralNet::getHiddenLayerActivation() {
    for( i = 0 ; i < HiddenNodes ; i++ ) {    
      Accum = HiddenWeights[InputNodes][i] ;
      for( j = 0 ; j < InputNodes ; j++ ) {
        Accum += Input[j] * HiddenWeights[j][i] ;
      }
      Hidden[i] = activationFunction(Accum);
    }
}

float NeuralNet::activationFunction(float inputValue) {
    // return 1.0/(1.0 + exp(-input * Alpha)) ;
    return 0.5 * inputValue * Alpha / (1 + abs(inputValue * Alpha)) + 0.5;
}

/******************************************************************
* Compute output layer activations and calculate errors
******************************************************************/
void  NeuralNet::getOutputLayerActivation() {
    for( i = 0 ; i < OutputNodes ; i++ ) {    
        Accum = OutputWeights[HiddenNodes][i] ;
        for( j = 0 ; j < HiddenNodes ; j++ ) {
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
    for( i = 0; i < HiddenNodes; i++ ) {    
        Accum = 0.0;
        
        for( j = 0; j < OutputNodes; j++ ) {
            Accum += OutputWeights[i][j] * OutputDelta[j];
        }
        
        HiddenDelta[i] = Accum * Hidden[i] * (1.0 - Hidden[i]);
    }

    // Update Inner-->Hidden Weights
    for( i = 0; i < HiddenNodes; i++ ) {     
        ChangeHiddenWeights[InputNodes][i] = LearningRate * HiddenDelta[i] + Momentum * ChangeHiddenWeights[InputNodes][i];
        HiddenWeights[InputNodes][i] += ChangeHiddenWeights[InputNodes][i];
    
        for( j = 0; j < InputNodes; j++ ) { 
            ChangeHiddenWeights[j][i] = LearningRate * TrainData[p][j] * HiddenDelta[i] + Momentum * ChangeHiddenWeights[j][i];
            HiddenWeights[j][i] += ChangeHiddenWeights[j][i];
        }
    }

    // Update Hidden-->Output Weights
    for( i = 0; i < OutputNodes; i ++ ) {    
        ChangeOutputWeights[HiddenNodes][i] = LearningRate * OutputDelta[i] + Momentum * ChangeOutputWeights[HiddenNodes][i];
        OutputWeights[HiddenNodes][i] += ChangeOutputWeights[HiddenNodes][i];
    
        for( j = 0; j < HiddenNodes; j++ ) {
            ChangeOutputWeights[j][i] = LearningRate * Hidden[j] * OutputDelta[i] + Momentum * ChangeOutputWeights[j][i];
            OutputWeights[j][i] += ChangeOutputWeights[j][i];
        }
    }
}

void NeuralNet::printInputData() {
    Serial->print ("Input: ");
    for( i = 0 ; i < InputNodes ; i++ ) {       
      Serial->print (Input[i]);
      Serial->print (" ");
    }
};
void NeuralNet::printOutputActivation() {
    Serial->println (" ");
    Serial->print ("    Hidden Nodes: ");

    for( i = 0 ; i < HiddenNodes ; i++ ) {       
      Serial->print (Hidden[i], 4);
      Serial->print (" ");
    }
    Serial->println (" ");

    Serial->print ("    Output: ");
    for( i = 0 ; i < OutputNodes ; i++ ) {       
      Serial->print (Output[i], 4);
      Serial->print (" ");
    }
};

float NeuralNet::randomWeight() {
    return float(random(100))/100;
}
