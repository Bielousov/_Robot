#include "LedControl.h"

/*
 * My eyes are an array of 2x 8x8 MAX72XX LED matrixes
 * -----
 * LED Matrix Pins
 * pin 12 is connected to the DataIn 
 * pin 11 is connected to the CLK 
 * pin 10 is connected to LOAD 
 * I have 2x MAX72XX.
 */
LedControl led = LedControl(
    PIN_EYES_LED_DATA,
    PIN_EYES_LED_CLK,
    PIN_EYES_LED_LOAD,
    2
);

#define LEFT_EYE    0   // Left eye LED position in array
#define RIGHT_EYE   1  // Right eye LED position in array

const byte EYES_OPEN_ANIMATION_SEQUENCE[5] = {0, 1, 2, 3, 4};
const byte EYES_CLOSE_ANIMATION_SEQUENCE[5] = {4, 3, 2, 1, 0};
const short EYES_BLINK_INTERVAL[2] = {300, 6000};

struct EYES_OPTIONS {
  byte brightness;
  byte fps;
};

EYES_OPTIONS eyesOptions = {
  1,        // LED Brightness
  1000/24   // Animation in FPS
};

// Eyes thread
ThreadController eyesThread = ThreadController();
Thread eyesAnimationThread = Thread();
Thread eyesBlinkThread = Thread();


void initEyes() {
  /*
   * The MAX72XX is in power-saving mode on startup,
   * we have to do a wakeup call
  */
  led.shutdown(LEFT_EYE, false);
  led.shutdown(RIGHT_EYE, false);

  /* Dim brightness */
  led.setIntensity(LEFT_EYE, eyesOptions.brightness);
  led.setIntensity(RIGHT_EYE, eyesOptions.brightness);

  /* Clear display */
  led.clearDisplay(LEFT_EYE);
  led.clearDisplay(RIGHT_EYE);

  /* Init animation threads */
  setupEyesThread();
 
  /* Start by opening my eyes */
  openEyes();
}


void closeEyes() {
  // Stop blinking when my eye are closed
  eyesThread.remove(&eyesBlinkThread);

  // Add animation sequence to queue
  addAnimationSequence(eyeBlinkAnimationBitmap, EYES_CLOSE_ANIMATION_SEQUENCE, sizeof(EYES_CLOSE_ANIMATION_SEQUENCE));
}

void openEyes() {
  // Add animation sequence to queue
  addAnimationSequence(eyeBlinkAnimationBitmap, EYES_OPEN_ANIMATION_SEQUENCE, sizeof(EYES_OPEN_ANIMATION_SEQUENCE));

  // When my eyes are open I want to blink
  makeBlinkDecision();
}


void makeBlinkDecision() {
  int openEyeDuration = random(EYES_BLINK_INTERVAL[0], EYES_BLINK_INTERVAL[1]);
  eyesBlinkThread.setInterval(openEyeDuration);
  eyesThread.add(&eyesBlinkThread);
}


/*
 * Thread events
 */

void onEyesAnimation() {
  if (eyesAnimationQueue.size() == 0) {
    return;
  }
  
  byte* frameBitmap = popFrameFromAnimationQueue();
  drawEyes(frameBitmap);
}

void onEyesBlink() {
  closeEyes();
  openEyes();
}

/*
 * Render functions
 */
void drawEyes(byte* bitmap) {
    drawEye(LEFT_EYE, bitmap);
    drawEye(RIGHT_EYE, bitmap);
}

void drawEye(byte eye, byte* bitmap) {
  for(int row=0; row<8; row++) {
      led.setRow(eye, row, bitmap[row]);
  }
}

/*
 * Thread process
 */
void setupEyesThread() {
  // Animation thread runs at 24 FPS
  eyesAnimationThread.onRun(onEyesAnimation);
  eyesAnimationThread.setInterval(eyesOptions.fps);
  eyesThread.add(&eyesAnimationThread);

  // Blink thread is a one off, added on demand
  eyesBlinkThread.onRun(onEyesBlink);
}

void runEyesThread() {
  if(eyesThread.shouldRun())
    eyesThread.run();
}

