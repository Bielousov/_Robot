#include <math.h>
#include <NeuralNet.h>

// Neural Network Settings
const uint8_t HiddenNodes = 16;
const uint8_t InputNodes = 4;
const uint8_t OutputNodes = 4;

void setup(){
  Serial.begin(9600);
  randomSeed(analogRead(3));

  initializeNeuralNetwork();
}  

void loop (){

}


