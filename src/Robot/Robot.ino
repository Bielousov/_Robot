#include <Thread.h>

// Used Pins
// #define PIN_PIR_SENSOR      3
#define PIN_EYES_LED_CS     10
#define PIN_EYES_LED_DIN    11
#define PIN_EYES_LED_CLK    13

// LED Matrix size
const uint8_t BITMAP_SIZE = 8;

void setup() {  
  //Serial.begin(9600);

  // Randomize the random function
  randomSeed(analogRead(0));
  
  initDecisions();
  initEyes();
}

void loop() {
  // Run asynchronous threads
  runDecisionsThread();
  runEyesThread();
}
