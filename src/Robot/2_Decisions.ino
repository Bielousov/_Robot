/* 
 * TODO: 
 * 1. Decisions priority queue
 */

// Decisions thread
Thread decisionThread = Thread();

const uint8_t DECISION_FREQUENCY = 200;   // Decisions frequency, in ms
const uint8_t DECISION_MAX_WEIGHT = 255;  // Decisions max weight

/*
 * Decisions model state
 * =====================
 */
struct DECISION_STATE {
    uint16_t timeSincePreviousDecision;
};

DECISION_STATE decisionState = {
    0, // Time since after previous decision made  
};

/*
 * Decisions
 * =========
 */

struct DECISION {
    bool allowed;
    uint16_t timeSincePreviousDecision;
    uint8_t weight;
};

/* 
 * Eyes: Blink Decisions 
 * ---------------------
 */
DECISION eyesBlink = {
    false,    // Blinking not allowed when eyes are closed
    0,        // Time in us since last time I opened my eyes
    0         // Default weight to blink
};
 
void makeDecisionEyesBlink(uint8_t weight = 0) {
    if (!eyesBlink.allowed) {
        return;
    }

    // Decision weight grows over time
    if (weight > 0) {
      eyesBlink.weight = weight;
    } else if (eyesBlink.weight < DECISION_MAX_WEIGHT) {
       eyesBlink.weight++;
    }
    
    makeDecision(&eyesBlink, onEyesBlink);
};

void resetEyesBlinkDecision(bool allowed) {
    if (eyesBlink.allowed == allowed) {
        return;
    }
  
    eyesBlink.allowed = allowed;
    eyesBlink.timeSincePreviousDecision = 0;
    eyesBlink.weight = 0;
}

/* 
 * Eyes: Pupils Move Decisions
 * ---------------------------
 */
DECISION eyesPupils = {
    true, // Pupils movement always allowed
    0,    // Time in us since last time I moved my pupils
    0     // Default weight to move pupils
};

void makeDecisionEyesPupilsMove(uint8_t weight = 0) {
    if (!eyesPupils.allowed) {
        return;
    }

    // Decision weight is random in a range [0 ... 10, %]
    eyesPupils.weight = weight > 0 ? weight : random(DECISION_MAX_WEIGHT/10);

    makeDecision(&eyesPupils, onPupilsMove);
}


/*
 * Common decissions logics
 * ========================
 */
void initDecisions() {
    // Decision thread runs at 5 Hz
    decisionThread.onRun(onDecision);
    decisionThread.setInterval(DECISION_FREQUENCY);
}

void makeDecision (struct DECISION *decision, void calback()) {
    bool decisionChance = decision->weight > random(DECISION_MAX_WEIGHT);
  
    // Negative decision
    if (!decisionChance) {
        decision->timeSincePreviousDecision += DECISION_FREQUENCY;
        return;
    }
  
    // Positive decision
    decisionState.timeSincePreviousDecision = 0;
    decision->timeSincePreviousDecision = 0;
    decision->weight = 0;
    calback();
}

/*
 * Asynchronous thread process
 * ===========================
 */
void runDecisionThread() {
  if(decisionThread.shouldRun()) {
      decisionThread.run();
  }
}

/* 
 * Thread callback 
 * ---------------
 */
void onDecision() {
  // Update decision state
  decisionState.timeSincePreviousDecision += DECISION_FREQUENCY;
  
  makeDecisionEyesBlink();
  makeDecisionEyesPupilsMove();
}

