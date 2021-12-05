#include <avr/pgmspace.h>
#include "Vocab_Special.h"
#include "Vocab_US_TI99.h"

// Voice dictionary
struct INTENT {
  const uint8_t* greeting[7] = { spt_HELLO, spPAUSE1, spt_HOW, spt_ARE, spt_YOU, spt_DOING, NULL };
};

// Text dictionary
const char STR_FREE_MEMORY[] PROGMEM = "Free memory: ";
const char STR_START[] PROGMEM = "Hello, I'm the Robot";

Dictionary dictionary;
