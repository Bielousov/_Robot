#include <avr/pgmspace.h>

// LED Matrix size
#define BITMAP_SIZE     8

// Eye animation frames
PROGMEM const byte eyeBitmap[1][BITMAP_SIZE] = {{
  B00111100,
  B01111110,
  B11111111,
  B11111111,
  B11111111,
  B11111111,
  B01111110,
  B00111100
}};

PROGMEM const byte eyeBlinkAnimationBitmap[][BITMAP_SIZE] = {
  { 
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B10000001,
    B01111110,
    B00000000,
    B00000000 
  }, { 
    B00000000,
    B00000000,
    B00000000,
    B00111100,
    B11111111,
    B01111110,
    B00011000,
    B00000000 
  }, { 
    B00000000,
    B00000000,
    B00111100,
    B11111111,
    B11111111,
    B11111111,
    B00111100,
    B00000000
  }, {
    B00000000,
    B01111110,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B01111110,
    B00111100
  }, { 
    B00111100,
    B01111110,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B01111110,
    B00111100
  }
};

/*
 * Helper method to load bitmap from PROGMEM
 * -----------------------------------------
 */
byte bitmapBuffer[BITMAP_SIZE];
byte* loadBitmap(const byte bitmap[][BITMAP_SIZE], uint8_t frameIndex) {

  for (uint8_t y = 0; y < BITMAP_SIZE; y++) {
    bitmapBuffer[y] = pgm_read_byte_near(&bitmap[frameIndex][y]);
  }

  return bitmapBuffer;
}

bool bitmapPixel;
byte loadBitmapPixel(const byte bitmap[BITMAP_SIZE], uint8_t x, uint8_t y) {
  bitmapPixel = pgm_read_byte_near(&bitmap[y]) & (1 << x);
  return bitmapPixel;
}
