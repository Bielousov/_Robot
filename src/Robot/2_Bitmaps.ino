#include <avr/pgmspace.h>

// Eye animation frames
PROGMEM const byte eyeBlinkAnimationBitmap[5][BITMAP_SIZE] = {
  { B00000000,
    B00000000,
    B00000000,
    B00000000,
    B10000001,
    B01111110,
    B00000000,
    B00000000 },
  { B00000000,
    B00000000,
    B00000000,
    B00111100,
    B11111111,
    B01111110,
    B00011000,
    B00000000 },
  { B00000000,
    B00000000,
    B00111100,
    B11111111,
    B11111111,
    B11111111,
    B00111100,
    B00000000 },
  { B00000000,
    B01111110,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B01111110,
    B00111100 },
  { B00111100,
    B01111110,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B01111110,
    B00111100 }
};

/*
 * Helper method to load bitmap from PROGMEM
 * -----------------------------------------
 */
byte* loadBitmapFromProgmem(const byte bitmap[][BITMAP_SIZE], uint8_t frameIndex) {
  static byte progmemBitmapBuffer[BITMAP_SIZE];

  for (uint8_t row = 0; row < BITMAP_SIZE; row++) {
    progmemBitmapBuffer[row] = pgm_read_byte_near(&bitmap[frameIndex][row]);
  }

  return progmemBitmapBuffer;
}
