#include "Talkie.h"

/*
 * Voice PWM output pins for different ATmegas:
 *  ATmega328 (Uno and Nano): non inverted at pin 3, inverted at pin 11.
 *  ATmega2560: non inverted at pin 6, inverted at pin 7.
 *  ATmega32U4 (Leonardo): non inverted at pin 10, inverted at pin 9.
 *  ATmega32U4 (CircuitPlaygound): only non inverted at pin 5.
 *
 *  As default both inverted and not inverted outputs are enabled to increase volume if speaker is attached between them.
 *  Use Talkie Voice(true, false); if you only need not inverted pin or if you want to use SPI on ATmega328 which needs pin 11.
 *
 *  The outputs can drive headphones directly, or add a simple audio amplifier to drive a loudspeaker.
 */

Talkie Voice;

INTENT Intent;

void initVoice() {
  #if defined(TEENSYDUINO)
    //Enable Amplified PROP shield
    pinMode(PIN_SOUND_OUT, OUTPUT);
    digitalWrite(PIN_SOUND_OUT, HIGH);
  #endif
}

void say(const uint8_t** intent) {
  uint8_t i = 0;
  while (intent[i]) {
    Voice.say(intent[i]);
    i++;
 }
}

void sayHello() {
  Serial.print(dictionary.get(STR_START));
  say(Intent.greeting);
}
