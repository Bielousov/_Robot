#include <Thread.h>
#include <ThreadController.h>

#define PIN_EYES_LED_LOAD   10
#define PIN_EYES_LED_CLK    11
#define PIN_EYES_LED_DATA   12

void setup() {
  // Serial.begin(9600);
  initEyes();
}

void loop() {
  runEyesThread();
}
