#include <Thread.h>
#include <ThreadController.h>

#define PIN_EYES_LED_CS     9
#define PIN_EYES_LED_DIN    11
#define PIN_EYES_LED_CLK    13

void setup() {
  // Serial.begin(9600);
  initEyes();
}

void loop() {
  runEyesThread();
}
