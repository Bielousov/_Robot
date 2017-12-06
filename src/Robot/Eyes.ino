#include <LEDMatrixDriver.hpp>

const byte EYES_OPEN_ANIMATION_SEQUENCE[5] = {0, 1, 2, 3, 4};
const byte EYES_CLOSE_ANIMATION_SEQUENCE[5] = {4, 3, 2, 1, 0};
const short EYES_BLINK_INTERVAL[2] = {300, 6000};

/*
 * My eyes are an array of 2x 8x8 MAX72XX LED matrixes
 * -----
 * LED Matrix Pins
 * pin 12 is connected to the DataIn 
 * pin 11 is connected to the CLK 
 * pin 10 is connected to LOAD 
 * I have 2x MAX72XX.
 */
LEDMatrixDriver lmd(1, PIN_EYES_LED_CS);

struct EYES_OPTIONS {
  byte brightness;
  byte fps;
};

EYES_OPTIONS eyesOptions = {
  0,        // LED Brightness
  1000/24   // Animation in FPS
};

// Eyes thread
ThreadController eyesThread = ThreadController();
Thread eyesAnimationThread = Thread();
Thread eyesBlinkThread = Thread();

/*
 * Setup
 */
void initEyes() {
  lmd.setEnabled(true);
  lmd.setIntensity(eyesOptions.brightness);
  lmd.clear();

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
  for(byte x=0; x<8; x++) {
    for(byte y=0; y<8; y++) {
      lmd.setPixel(x, y, bitmap[x] & (1 << y));
    }
  }

  lmd.display();
}

/*
 * Asynchronous thread process
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
  if(eyesThread.shouldRun()) {
    eyesThread.run();
  }
}

