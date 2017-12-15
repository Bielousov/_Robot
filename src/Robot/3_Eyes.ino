#include <LEDMatrixDriver.hpp>
 
// Eyes thread
Thread eyesAnimationThread = Thread();

const uint8_t EYES_ANIMATION_FPS = 1000 / 24;    // Animation frequency, in FPS -> ms
const uint8_t EYES_BRIGHTNESS = 0;             // LED Brightness
const uint8_t EYES_ORIGIN_POSITION = 8 / 2 - 1;         // Define LED matrix center in 0-based coordinates (3)

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
    
    /* Animation thread runs at 24 FPS */
    eyesAnimationThread.onRun(onEyesAnimation);
    eyesAnimationThread.setInterval(EYES_ANIMATION_FPS);
  
    /* Start by opening my eyes */
    State.Eyes.currentFrame = (byte*)eyeBlinkAnimationBitmap[0];
    openEyes();
}


void closeEyes() {
    // Stop blinking when my eye are closed
    resetEyesBlinkDecision(false);
  
    // Add animation sequence to queue
    addAnimationSequence(eyeBlinkAnimationBitmap, EYES_CLOSE_ANIMATION_SEQUENCE, sizeof(EYES_CLOSE_ANIMATION_SEQUENCE));
}

void openEyes() {
    // Add animation sequence to queue
    addAnimationSequence(eyeBlinkAnimationBitmap, EYES_OPEN_ANIMATION_SEQUENCE, sizeof(EYES_OPEN_ANIMATION_SEQUENCE));
  
    // When my eyes are open I want to blink
    resetEyesBlinkDecision(true);
}

/*
 * Blinking
 * ========
 */
void onEyesBlink() {
    closeEyes();
    openEyes();
}

/*
 * Pupils
 * ======
 */
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

void onPupilsMove() {
    State.Eyes.pupilsPosition[0] = random(-2, 2);
    State.Eyes.pupilsPosition[1] = random(-2, 2);

    if (isAnimationQueueEmpty()) {
      pushFrameToAnimationQueue(State.Eyes.currentFrame);
    }
    
}

/*
 * LED Rendering
 * =============
 */
void drawEyes(byte* bitmap) {  
    for(uint8_t x=0; x<8; x++) {
        for(uint8_t y=0; y<8; y++) {
            // Get each led pixel and apply masks to it
            bool ledPixel = bitmap[y] & (1 << x);
            ledPixel = applyPupilMask(x, y, ledPixel);
      
            // Rotate matrix CCW
            uint8_t ledX = y;
            uint8_t ledY = 7 - x;
            lmd.setPixel(ledX, ledY, ledPixel);
        }
    }

    State.Eyes.currentFrame = bitmap;
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
    if (isAnimationQueueEmpty()) {
        return;
    }
    
    byte* frameBitmap = popFrameFromAnimationQueue();
    drawEyes(frameBitmap);
}

