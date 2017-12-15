/* 
 * States Definition
 * =====================
 */
 
// Decisions state
struct DECISION_STATE {
    uint16_t timeSincePreviousDecision = 0;
};

// Environment state
struct ENVIRONMENT_STATE {
  bool isHumanDetected = false;
  uint16_t timeWhenHumanArrived = 0;
  uint16_t timeWhenHumanDeparted = 0;
};

// Eyes state
struct EYES_STATE {
    bool isOpen = false;
    uint8_t pupilsPosition[2] = {0, 0};
    uint8_t pupilsSize[2] = {2, 2};
    byte* currentFrame;
};

// Global state store
struct STATE {
  DECISION_STATE Decision = {};
  ENVIRONMENT_STATE Environment = {};
  EYES_STATE Eyes = {};
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
