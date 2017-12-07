#include <Thread.h>

// Used Pins
#define PIN_EYES_LED_CS     9
#define PIN_EYES_LED_DIN    11
#define PIN_EYES_LED_CLK    13

void setup() {
  // Serial.begin(9600);
  initDecisions();
  initEyes();
}

void loop() {
  // Run asynchronous threads
  runDecisionsThread();
  runEyesThread();
}
