#include <AnimationBuffer.h>
#include <LEDMatrixDriver.hpp>

const uint8_t EYES_SEGMENTS = 1;                                      // How many eyes I have
const uint8_t EYES_ANIMATION_FPS = 30;                                // Animation frequency, in FPS
const uint8_t EYES_ANIMATION_INTERVAL = 1000 / EYES_ANIMATION_FPS;    // Animation frequency, in ms
const uint8_t EYES_ANIMATION_SPEED_BLINK_MULTIPLIER = 1;              // Blink animation speed multiplier
const uint8_t EYES_ANIMATION_SPEED_PUPILS_MULTIPLIER = 3;             // Pupils movement animation speed multiplier
const uint8_t EYES_BRIGHTNESS = 2;                                    // LED Brightness
const uint8_t EYES_ORIGIN_POSITION = BITMAP_SIZE / 2 - 1;             // Define LED matrix center in 0-based coordinates (3)

// Animation frame sequences
const uint8_t EYES_OPEN_ANIMATION_SEQUENCE[5] = {1, 2, 3, 4, 5};
const uint8_t EYES_CLOSE_ANIMATION_SEQUENCE[5] = {5, 4, 3, 2, 1};


/*
 * My eyes are an array of 2x 8x8 MAX72XX LED matrixes
 */
LEDMatrixDriver lmd(EYES_SEGMENTS, PIN_EYES_LED_CS);

// Eyes animation thread
Thread eyesAnimationThread = Thread(onEyesAnimation, EYES_ANIMATION_INTERVAL);

AnimationBuffer animationBuffer;


/*
 * Setup
 * =====
 */
void initEyes() {
  lmd.setEnabled(true);
  lmd.setIntensity(EYES_BRIGHTNESS);
  lmd.clear();

  /* Start by opening my eyes */
  openEyes();
}


void closeEyes() {
  // Update state
  State.Eyes.isOpened = false;

  // Stop blinking when my eye are closed
  updateDecisionsOnEyesClosed();

  // Add animation sequence to queue
  generateEyesBlinkAnimationFramesSequence(EYES_CLOSE_ANIMATION_SEQUENCE);
}

void openEyes() {
  // Update state
  State.Eyes.isOpened = true;

  // Add animation sequence to queue
  generateEyesBlinkAnimationFramesSequence(EYES_OPEN_ANIMATION_SEQUENCE);
}

/*
 * Blinking
 * ========
 */
void blinkEyes() {
    closeEyes();
    openEyes();
}

/*
 * Pupils
 * ======
 */
void moveEyes() {
  int8_t* newPupilsPosition = State.Eyes.pointOfInterest;
  
  while (State.Eyes.pupilsPosition[0] != newPupilsPosition[0] || State.Eyes.pupilsPosition[1] != newPupilsPosition[1]) {
    // Move pupil only 1px in X and Y axis at once until the final position is reached
    for (uint8_t i = 0; i < 2; i++) {
       if(State.Eyes.pupilsPosition[i] < newPupilsPosition[i]) {
          State.Eyes.pupilsPosition[i]++;
       } else if(State.Eyes.pupilsPosition[i] > newPupilsPosition[i]) {
          State.Eyes.pupilsPosition[i]--;
       }

       generateEyesFrame(EYES_ANIMATION_SPEED_PUPILS_MULTIPLIER);
    }
  }
}

/*
 * Set point of interest
 * =====================
 * Sets a focus point that would attract eyes movemet
 */

void setPointOfInterest (int8_t h, int8_t v) {
  State.Eyes.pointOfInterest[0] = h;
  State.Eyes.pointOfInterest[1] = v;
}

void setRandomPointOfInterest () {
   setPointOfInterest ((int8_t)random(-2, 2), (int8_t)random(-2, 2));
}

/*
 * LED Rendering
 * =============
 */
bool applyLashMask(uint8_t x, uint8_t y, bool ledPixel) {
  // byte* lashMaskBitmap = loadBitmap(eyeBlinkAnimationBitmap, State.Eyes.lashesPosition - 1);
  bool lashMaskPixel = loadBitmapPixel(eyeBlinkAnimationBitmap[State.Eyes.lashesPosition - 1], x, y);
  bool lashMaskTopPixel = y > 0 ? loadBitmapPixel(eyeBlinkAnimationBitmap[State.Eyes.lashesPosition - 1], x, y - 1) : false;
  bool lashMaskBottomPixel = y < BITMAP_SIZE - 1 ? loadBitmapPixel(eyeBlinkAnimationBitmap[State.Eyes.lashesPosition - 1], x, y + 1) : false;
  
  bool isMaskEdge = lashMaskTopPixel != lashMaskPixel || lashMaskBottomPixel != lashMaskPixel;

  return !lashMaskPixel || (State.Eyes.isAnimating && isMaskEdge)  ? lashMaskPixel : ledPixel;
    
}

bool applyPupilMask(uint8_t x, uint8_t y, bool ledPixel) { 
  if (!ledPixel) {
      return ledPixel;
  }

  uint8_t xStartPosition = EYES_ORIGIN_POSITION + State.Eyes.pupilsPosition[0];
  uint8_t xEndPosition = xStartPosition + State.Eyes.pupilsSize[0] - 1;
  uint8_t yStartPosition = EYES_ORIGIN_POSITION + State.Eyes.pupilsPosition[1];
  uint8_t yEndPosition = yStartPosition + State.Eyes.pupilsSize[1] - 1;

  if (x >= xStartPosition && x <= xEndPosition && y >= yStartPosition && y <= yEndPosition) {
    // Round edges on large pupils
    if(State.Eyes.pupilsSize[0] > 2 && State.Eyes.pupilsSize[1] > 2) {
      if ((x == xStartPosition || x == xEndPosition) && (y == yStartPosition || y == yEndPosition)) {
        return ledPixel;
      } 
    }     
    return false;
  }
    
  return ledPixel;
}

void generateEyesBlinkAnimationFramesSequence(
    const uint8_t* animationSequence
) {
  uint8_t i = 0;
  while(animationSequence[i]) {
      State.Eyes.lashesPosition = animationSequence[i];
      generateEyesFrame(EYES_ANIMATION_SPEED_BLINK_MULTIPLIER);
      i++;
  }
}

void generateEyesFrame(uint8_t animationDelay) {
  byte* frameBuffer = animationBuffer.getFrameBuffer();
  byte* frameBitmap = loadBitmap(eyeBitmap, 0);

  memcpy(frameBuffer, frameBitmap, BITMAP_SIZE);
  
  for (uint8_t x = 0; x < BITMAP_SIZE; x++) {
      for (uint8_t y = 0; y < BITMAP_SIZE; y++) {
          // Get each led pixel and apply masks to it
          bool ledPixel = frameBuffer[y] & (1 << x);
          ledPixel = applyPupilMask(x, y, ledPixel);
          ledPixel = applyLashMask(x, y, ledPixel);

          bitWrite(frameBuffer[y], x, ledPixel);
      }
  }

  // Slow down animation
  repeatFrame(frameBuffer, animationDelay);
}

// Slow down animation
void repeatFrame(byte* frameBitmap, uint8_t frameDelay) {
  for (uint8_t t = 0; t < frameDelay; t++) {
    animationBuffer.addFrame(frameBitmap);
  }
}

void drawEyes(byte* bitmap) {  
  for (uint8_t x = 0; x < BITMAP_SIZE; x++) {
    for (uint8_t y = 0; y < BITMAP_SIZE; y++) {
      // Get each led pixel and apply masks to it
      bool ledPixel = bitmap[y] & (1 << x);

      // Rotate matrix CCW
      lmd.setPixel(y, BITMAP_SIZE - x - 1, ledPixel);
    }
  }
  
  lmd.display();
}


/*
 * Asynchronous thread process
 * ===========================
 */
void runEyesAnimationThread() {
  if(eyesAnimationThread.shouldRun()) {
    eyesAnimationThread.run();
  }
}

/*
 * Thread callback
 * ---------------
 */
void onEyesAnimation() {
  if (animationBuffer.isQueueEmpty()) {
    State.Eyes.isAnimating = false;
    return;
  }

  State.Eyes.isAnimating = true;
  byte* frameBitmap = animationBuffer.getFrame();
  drawEyes(frameBitmap);
}
