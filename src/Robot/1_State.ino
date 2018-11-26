/* 
 * States Definition
 * =====================
 */

// Eyes state
struct EYES_STATE {
    bool    isOpened          = false;  // Eye lashes state
    uint8_t lashesPosition     = 0;      // Eye lashes position as animation frame number
    int8_t pointOfInterest[2] = {0, 0}; // Focus gravitation
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
