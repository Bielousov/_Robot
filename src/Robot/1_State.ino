/* 
 * States Definition
 * =====================
 */

// Eyes state
struct EYES_STATE {
    byte*   currentFrame;               // Current frame pointer
    bool    isOpened          = false;  // Eye lashes state
    int8_t focus[2]           = {0, 0}; // Focus gravitation
    int8_t pupilsPosition[2]  = {0, 0}; // Current focus
    uint8_t pupilsSize[2]     = {2, 2}; // Pupils size
};

// Global state store
struct STATE {
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
