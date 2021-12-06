
#include <math.h>
#include <NeuralNet.h>

// Neural Network Settings
const uint8_t InputNodes = 1;
const uint8_t HiddenNodes = 16;
const uint8_t OutputNodes = 4;

static uint8_t i;
static long start;
static long finish;

//const uint8_t TrainDataSize = 7;
//PROGMEM const float TrainData[TrainDataSize][InputNodes] = {
//  { 1, 0, 0, 0 },
//  { 0, 0, 0, 0 },
//  { 0, 0, 0, 1 },
//  { 0, 1, 0, 1 },
//  { 0, 1, 1, 1 },
//  { 1, 1, 1, 1 },
//  { 1, 0, 0, 1 }
//}; 
//
//PROGMEM const float TargetData[TrainDataSize][OutputNodes] = {
//  { 0, 1, 0, 0, 0 },  
//  { 0, 0, 0, 0, 1.0 }, 
//  { 0, 0, 0.5, 0, 0 }, 
//  { 0, 0, 0, 0.5, 0 }, 
//  { 0.5, 0, 0, 0, 0 }, 
//  { 1, 0, 0, 0, 0 }, 
//  { 0, 0.5, 0, 0, 0 }
//};
//
//
//const uint8_t TestDataSize = 6;
//PROGMEM const float TestData[TestDataSize][InputNodes] = {
//  { 0, 0.75, 0.66, 0 },
//  { 0, 0.2, 0.9, 0.9 },
//  { 0, 0.8, 0.2, 0.8 },
//  { 0, 0.8, 0.5, 0.2 },
//  { 1, 0.1, 0.5, 0.2 },
//  { 1, 0.01, 0.01, 0.2 }  
//};

const uint8_t TrainDataSize = 6;
PROGMEM const float TrainData[TrainDataSize][InputNodes] = {
  { 0.0 },
  { 1.0 },
  { 0.8 },
  { 0.6 },
  { 0.4 },
  { 0.2 },
}; 

PROGMEM const float TargetData[TrainDataSize][OutputNodes] = {
  { 0, 1, 0, 0 },
  { 0, 0, 0.1, 0.3},
  { 0, 0, 0.2, 0.2 }, 
  { 0, 0, 0.4, 0.1 }, 
  { 0, 0, 0.5, 0.05 }, 
  { 0.2, 0.3, 0, 0 }, 
};


const uint8_t TestDataSize = 6;
PROGMEM const float TestData[TestDataSize][InputNodes] = {
  { 0 },
  { 0.1 },
  { 0.3 },
  { 0.5 },
  { 0.7 },
  { 0.9 }  
};

void setup(){
  Serial.begin(9600);
  randomSeed(analogRead(3));

  delay(500);
  
  // Initialize neural network
  NeuralNet neuralNet = NeuralNet(InputNodes, HiddenNodes, OutputNodes, &Serial);
  neuralNet.ReportFrequencey = 100;
  neuralNet.Success = 0.05;
  
  // Train network
  neuralNet.train((float*)TrainData, (float*) TargetData, TrainDataSize);
  neuralNet.test((float*)TestData, sizeof(&TestData) * sizeof(float));
}  

void loop (){}