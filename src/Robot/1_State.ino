/* 
 * States Definition
 * =====================
 */

// Environment state
struct ENVIRONMENT_STATE {
  bool isHumanDetected          = true;
  int8_t pointOfInterest[2]     = {0, 0}; // corresponds to State.Eyes.pupilsPosition
  uint16_t timeHumanDetected    = 0;
  uint16_t timeHumanLost        = 0;
};

// Eyes state
struct EYES_STATE {
    byte*   currentFrame;
    bool    isOpened          = false;
    int8_t pupilsPosition[2]  = {0, 0};
    uint8_t pupilsSize[2]     = {2, 2};
};

// Global state store
struct STATE {
  ENVIRONMENT_STATE Environment;
  EYES_STATE Eyes;
};

/* 
 * States Initialization
 * =====================
 */
STATE State = {};


/*
 * Public Methods
 */
uint16_t getTime() {
  return millis() / 1000;
}

int8_t getRandomPointOfInterest () {
   return random(-2, 2);
}
