#include <math.h>
#include <NeuralNet.h>

// Neural Network Settings
const uint8_t InputNodes = 6;
const uint8_t HiddenNodes = 4;
const uint8_t OutputNodes = 2;

void setup(){
  Serial.begin(9600);
  randomSeed(analogRead(3));

  initializeNeuralNetwork();
}  

void loop (){

}
