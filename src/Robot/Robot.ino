#include <Arduino.h>
#include <Dictionary.h>
#include <NeuralNet.h>
#include <Thread.h>

/* -----
   MAX7219 LED Matrix Pins
   DIN — pin 51 (MOSI)
   CLK – pin 52 (SCK)
   CS – pin 49 (NOT MISO nor SS)
*/
// Used Pins
#define PIN_EYES_LED_CS     49
#define PIN_EYES_LED_DIN    51
#define PIN_EYES_LED_CLK    52
#define PIN_SOUND_OUT       6

/*
   Config
*/
#define LOG_ENABLED     false

void setup() {
  Serial.begin(9600);

  // Randomize the random function
  randomSeed(analogRead(0));

  initEyes();
  initVoice();
}

void loop() {
  // Run asynchronous threads
  runDecisionsThread();
  runEyesAnimationThread();
}
