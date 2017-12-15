/* 
 * States Definition
 * =====================
 */

/*
 * Decisions
 * =========
 */
struct DECISION {
    uint16_t  timeSincePreviousDecision = 0;
    uint8_t   weight = 0;
};

struct DECISION_STATE {
  DECISION eyesBlink;
  DECISION eyesPupils;
  DECISION sleepMode;
};


// Environment state
struct ENVIRONMENT_STATE {
  bool isHumanDetected          = true;
  uint16_t timeHumanDetected    = 0;
  uint16_t timeHumanLost        = 0;
};

// Eyes state
struct EYES_STATE {
    byte*   currentFrame;
    bool    isOpened          = false;
    uint8_t pupilsPosition[2] = {0, 0};
    uint8_t pupilsSize[2]     = {2, 2};
};

// Global state store
struct STATE {
  DECISION_STATE Decision;
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
