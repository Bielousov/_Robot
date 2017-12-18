/*
   TODO:
   1. Decisions priority queue
*/

// Decisions thread
Thread decisionThread = Thread();

const uint8_t DECISION_INTERVAL        = 250;   // Decisions frequency, in ms
const uint8_range DECISION_WEIGHT_RANGE = {0, 255}; // Decisions min / max weight

/*
   Eyes: Blink Decisions
   ---------------------
*/
void makeDecisionEyesBlink() {
  if (State.Eyes.isOpened) {
    incrementWeight(&State.Decision.eyesBlink);
    makeDecision(&State.Decision.eyesBlink, onEyesBlink);
  }
};

void resetEyesBlinkDecision() {
  resetDecision(&State.Decision.eyesBlink);
}

/*
   Eyes: Pupils Move Decisions
   ---------------------------
*/
void makeDecisionEyesPupilsMove() {
  incrementWeight(&State.Decision.eyesPupils);
  makeDecision(&State.Decision.eyesPupils, onPupilsMove);
}


/*
   Global: Sleep / Awakening State
   -------------------------------
*/
const uint8_range sleepModeRange = {30, 255}; // Min / Max time to fall asleep if no humans detected around

void makeDecisionToSleep() {
  if (isHumanDetected()) {
    makeDecisionInRange(&State.Decision.sleepMode, getTimeSinceHumanDetected(), sleepModeRange.min, sleepModeRange.max, onHumanLost);
  }
}

/*
   Common decissions logics
   ========================
*/
void initDecisions() {
  // Decision thread runs at 5 Hz
  decisionThread.onRun(onDecision);
  decisionThread.setInterval(DECISION_INTERVAL);
}

void incrementWeight(struct DECISION *decision) {
  if (decision -> weight < DECISION_WEIGHT_RANGE.max) {
    decision -> weight++;
  }
}

void setWeight(struct DECISION *decision, uint8_t weight) {
  decision -> weight = weight;
}

void makeDecision (struct DECISION *decision, void callback()) {
  bool decisionChance = decision -> weight > random(DECISION_WEIGHT_RANGE.max);

  // Negative decision
  if (!decisionChance) {
    decision -> timeSincePreviousDecision += DECISION_INTERVAL;
    return;
  }

  // Positive decision
  resetDecision(decision);
  callback();
}

void makeDecisionInRange (struct DECISION *decision, uint16_t value, uint16_t minValue, uint16_t maxValue, void callback()) {
  if (value < minValue) {
    return;
  }

  uint8_t weight = (uint8_t) (DECISION_WEIGHT_RANGE.max * (value - minValue) / (maxValue - minValue));
  setWeight(decision, weight);

  makeDecision(decision, callback);
}

void resetDecision(struct DECISION *decision) {
  decision -> timeSincePreviousDecision = 0;
  decision -> weight = 0;
}

/*
   Asynchronous thread process
   ===========================
*/
void runDecisionsThread() {
  if (decisionThread.shouldRun()) {
    decisionThread.run();
  }
}

/*
   Thread callback
   ---------------
*/
void onDecision() {
  makeDecisionEyesBlink();
  makeDecisionEyesPupilsMove();
  makeDecisionToSleep();
}

