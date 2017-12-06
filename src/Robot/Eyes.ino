#include <LEDMatrixDriver.hpp>

const byte EYE_ORIGIN_POSITION = 3;
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

/*
 * Default eyes options
 */
struct EYES_OPTIONS {
  byte brightness;
  byte fps;
};

EYES_OPTIONS eyesOptions = {
  0,        // LED Brightness
  1000/24   // Animation in FPS
};

/*
 * Default pupils properties
 */
struct PUPILS {
  byte size[2];
  int8_t position[2];
};

PUPILS pupils = {
  {2, 2},   // Pupils size
  {0, 1}   // Pupils position, center based
};


// Eyes thread
ThreadController eyesThread = ThreadController();
Thread eyesAnimationThread = Thread();
Thread eyesBlinkThread = Thread();
Thread eyesPupilsThread = Thread();

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
  onPupilsMove();
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


/*
 * Blinking
 */
void makeBlinkDecision() {
  int openEyeDuration = random(EYES_BLINK_INTERVAL[0], EYES_BLINK_INTERVAL[1]);
  eyesBlinkThread.setInterval(openEyeDuration);
  eyesThread.add(&eyesBlinkThread);
}

void onEyesBlink() {
  closeEyes();
  openEyes();
}

/*
 * Pupils functions
 */
bool applyPupilMask(byte x, byte y, bool ledPixel) { 
  if (!ledPixel) {
    return ledPixel;
  }

  byte xStartPosition = EYE_ORIGIN_POSITION + pupils.position[0];
  byte xEndPosition = xStartPosition + pupils.size[0];
  byte yStartPosition = EYE_ORIGIN_POSITION + pupils.position[1];
  byte yEndPosition = yStartPosition + pupils.size[1];

  if (x >= xStartPosition && x < xEndPosition && y >= yStartPosition && y < yEndPosition) {
    return false;
  }
    
  return ledPixel; 
}

void makePupilsDecision() {
  int pupilsMoveDuration = random(EYES_BLINK_INTERVAL[0], EYES_BLINK_INTERVAL[1]);
  eyesPupilsThread.setInterval(pupilsMoveDuration);
  eyesThread.add(&eyesPupilsThread);
}


void onPupilsMove() {
  eyesThread.remove(&eyesPupilsThread);
  
  pupils.position[0] = random(-2, 2);
  pupils.position[1] = random(-2, 2);

  makePupilsDecision();
}

/*
 * Render functions
 */
void drawEyes(byte* bitmap) {  
  for(byte x=0; x<8; x++) {
    for(byte y=0; y<8; y++) {
      // Get each led pixel and apply masks to it
      bool ledPixel = bitmap[y] & (1 << x);
      ledPixel = applyPupilMask(x, y, ledPixel);

      // Rotate matrix CCW
      byte ledX = y;
      byte ledY = 7 - x;
      lmd.setPixel(ledX, ledY, ledPixel);
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

  // Eye event threads are one offs, added on demand
  eyesBlinkThread.onRun(onEyesBlink);
  eyesPupilsThread.onRun(onPupilsMove);
}

void runEyesThread() {
  if(eyesThread.shouldRun()) {
    eyesThread.run();
  }
}

