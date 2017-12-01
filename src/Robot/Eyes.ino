#include "LedControl.h"

#define LEFT_EYE 0   // Left eye LED position in array
#define RIGHT_EYE 1  // Right eye LED position in array

const int EYES_BRIGHTNESS = 1; // LED brightness {1 ... 16}
const int BLINK_ANIMATION_DURATION = 1000/30; // fps
const int BLINK_CLOSED_DURATION = 200; // closed eye delay

const int eyeBlinkAnimationLength = 3;

byte pupilSize[2] = {2, 3}; // pupil size
byte pupilPosition[2] = {0, 0}; // pupil position


/*
 * My eyes are an array of 2x 8x8 MAX72XX LED matrixes
 * -----
 * LED Matrix Pins
 * pin 12 is connected to the DataIn 
 * pin 11 is connected to the CLK 
 * pin 10 is connected to LOAD 
 * I have 2x MAX72XX.
 */
LedControl led = LedControl(12,11,10, 2);

// Eyes thread
ThreadController eyesAnimation = ThreadController();
Thread eyesBlinkThread = Thread();

void setupEyes() {
  /*
   * The MAX72XX is in power-saving mode on startup,
   * we have to do a wakeup call
  */
  led.shutdown(LEFT_EYE, false);
  led.shutdown(RIGHT_EYE, false);

  /* Dim brightness */
  led.setIntensity(LEFT_EYE, EYES_BRIGHTNESS);
  led.setIntensity(RIGHT_EYE, EYES_BRIGHTNESS);

  /* Clear display */
  led.clearDisplay(LEFT_EYE);
  led.clearDisplay(RIGHT_EYE);

  /* Init animation threads */
  eyesBlinkThread.onRun(blinkEyes);
  
  openEyes();
}
void closeEyes() {
  drawEyesBlinkAnimation(0, eyeBlinkAnimationLength - 1);
  drawEyes(eyeClosedBitmap);
}

void openEyes() {
  drawEyesBlinkAnimation(eyeBlinkAnimationLength - 1, 0);
  drawEyes(eyeOpenedBitmap);

  // When my eyes are open I want to blink
  int openEyeDuration = random(1500, 6000);
  eyesBlinkThread.setInterval(openEyeDuration);
  eyesAnimation.add(&eyesBlinkThread);
 }

void blinkEyes() {
  eyesAnimation.remove(&eyesBlinkThread);
  
  closeEyes();

  delay(BLINK_CLOSED_DURATION);
  
  openEyes();
}

/*
 * Render functions
 */
 void drawEyes(const byte* bitmap) {
    led.setBitmap(LEFT_EYE, bitmap);
    led.setBitmap(RIGHT_EYE, bitmap);
    // drawPupils();
}

void drawEyesBlinkAnimation(int startFrame, int endFrame) {
  int frame = startFrame;

  while (frame != endFrame) {
    drawEyes(eyeBlinkAnimation[frame]);
    delay(BLINK_ANIMATION_DURATION);

    if (startFrame < endFrame) {
      frame++;
    } else {
      frame--;
    }
  }
}

void drawPupils() {
    byte pupilPositionLeft = 7 / 2 + pupilPosition[0];
    byte pupilPositionTop = 7 / 2 + pupilPosition[1];
    
    led.setRectangle(LEFT_EYE, pupilPositionTop, pupilPositionLeft, pupilSize[0], pupilSize[1], false);
    led.setRectangle(RIGHT_EYE, pupilPositionTop, pupilPositionLeft, pupilSize[0], pupilSize[1], false);
}

void runEyesThread() {
  if(eyesAnimation.shouldRun())
    eyesAnimation.run();
}

