#include <AnimationBuffer.h>
#include <LEDMatrixDriver.hpp>
 
// Eyes thread
Thread eyesAnimationThread = Thread();

AnimationBuffer animationBuffer;

const uint8_t EYES_ANIMATION_FPS = 20;                                // Animation frequency, in FPS
const uint8_t EYES_ANIMATION_INTERVAL = 1000 / EYES_ANIMATION_FPS;    // Animation frequency, in ms
const uint8_t EYES_ANIMATION_SPEED_BLINK_MULTIPLIER = 1;              // Blink animation speed multiplier
const uint8_t EYES_ANIMATION_SPEED_PUPILS_MULTIPLIER = 3;             // Pupils movement animation speed multiplier
const uint8_t EYES_BRIGHTNESS = 0;                                    // LED Brightness
const uint8_t EYES_ORIGIN_POSITION = BITMAP_SIZE / 2 - 1;             // Define LED matrix center in 0-based coordinates (3)

// Animation frame sequences
const uint8_t EYES_OPEN_ANIMATION_SEQUENCE[5] = {0, 1, 2, 3, 4};
const uint8_t EYES_CLOSE_ANIMATION_SEQUENCE[5] = {4, 3, 2, 1, 0};

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
 * Setup
 * =====
 */
void initEyes() {
    lmd.setEnabled(true);
    lmd.setIntensity(EYES_BRIGHTNESS);
    lmd.clear();
    
    /* Animation thread runs at 20 FPS */
    eyesAnimationThread.onRun(onEyesAnimation);
    eyesAnimationThread.setInterval(EYES_ANIMATION_INTERVAL);
  
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
    uint8_t blinkDelay = random(5);
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
  byte* lashMaskBitmap = loadBitmapFromProgmem(eyeBlinkAnimationBitmap, State.Eyes.lashesPosition);
  bool lashMaskPixel = lashMaskBitmap[y] & (1 << x);
  bool lashMaskTopPixel = y > 0 ? lashMaskBitmap[y - 1] & (1 << x) : false;
  bool lashMaskBottomPixel = y < BITMAP_SIZE - 1 ? lashMaskBitmap[y + 1] & (1 << x) : false;
  
  bool isAnimating = State.Eyes.lashesPosition > 0 && State.Eyes.lashesPosition < EYE_BLINK_ANIMATION_SEQUENCE_SIZE - 1;
  bool isMaskEdge = lashMaskTopPixel != lashMaskPixel || lashMaskBottomPixel != lashMaskPixel;

  return !lashMaskPixel || (isAnimating && isMaskEdge)  ? lashMaskPixel : ledPixel;
    
}

bool applyPupilMask(uint8_t x, uint8_t y, bool ledPixel) { 
    if (!ledPixel) {
        return ledPixel;
    }
  
    uint8_t xStartPosition = EYES_ORIGIN_POSITION + State.Eyes.pupilsPosition[0];
    uint8_t xEndPosition = xStartPosition + State.Eyes.pupilsSize[0];
    uint8_t yStartPosition = EYES_ORIGIN_POSITION + State.Eyes.pupilsPosition[1];
    uint8_t yEndPosition = yStartPosition + State.Eyes.pupilsSize[1];
  
    if (x >= xStartPosition && x < xEndPosition && y >= yStartPosition && y < yEndPosition) {
        return false;
    }
      
    return ledPixel; 
}

void generateEyesBlinkAnimationFramesSequence(
    const uint8_t* animationSequence
) {
    for (uint8_t i = 0; i < EYE_BLINK_ANIMATION_SEQUENCE_SIZE; i++) {
        State.Eyes.lashesPosition = animationSequence[i];
        generateEyesFrame(EYES_ANIMATION_SPEED_BLINK_MULTIPLIER);
    }
}

void generateEyesFrame(uint8_t animationDelay) {
    byte* frameBuffer = animationBuffer.getFrameBuffer();
    byte* frameBitmap = loadBitmapFromProgmem(eyeBitmap, 0);

    memcpy(frameBuffer, frameBitmap, BITMAP_SIZE);
    
    for(uint8_t x = 0; x < BITMAP_SIZE; x++) {
        for(uint8_t y = 0; y < BITMAP_SIZE; y++) {
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
    for(uint8_t x = 0; x < BITMAP_SIZE; x++) {
        for(uint8_t y = 0; y < BITMAP_SIZE; y++) {
            // Get each led pixel and apply masks to it
            bool ledPixel = bitmap[y] & (1 << x);
      
            // Rotate matrix CCW
            uint8_t ledX = y;
            uint8_t ledY = BITMAP_SIZE - x - 1;
            lmd.setPixel(ledX, ledY, ledPixel);
        }
    }
    
    lmd.display();
}


/*
 * Asynchronous thread process
 * ===========================
 */
void runEyesThread() {
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
        return;
    }
    
    byte* frameBitmap = animationBuffer.getFrame();
    drawEyes(frameBitmap);
}
