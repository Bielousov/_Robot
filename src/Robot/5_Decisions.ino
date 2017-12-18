#include <Decision.h>

const uint8_t DECISION_INTERVAL        = 200;   // Decisions frequency, in ms

struct DECISION_STATE {
  Decision eyesBlink;
  Decision eyesMove;
  Decision sleepMode;
};

DECISION_STATE DecisionState;

// Decisions thread
Thread decisionThread = Thread();

/*
   Eyes: Blink Decisions
   ---------------------
*/
void makeEyesBlinkDecision() {
  if (!State.Eyes.isOpened) {
    DecisionState.eyesBlink.incrementWeight();
    DecisionState.eyesBlink.makeDecision(onEyesBlink);
  }
};

void resetEyesBlinkDecision() {
  DecisionState.eyesBlink.resetDecision();
}

/*
   Eyes: Pupils Move Decisions
   ---------------------------
*/
void makeEyesMoveDecision() {
  DecisionState.eyesMove.incrementWeight();
  DecisionState.eyesMove.makeDecision(onPupilsMove);
}


/*
   Global: Sleep / Awakening State
   -------------------------------
*/
const uint8_range sleepModeRange = {30, 255}; // Min / Max time to fall asleep if no humans detected around

void makeSleepDecision() {
  if (isHumanDetected()) {
    DecisionState.sleepMode.setWeightInRange(getTimeSinceHumanDetected(), sleepModeRange.min, sleepModeRange.max);
    DecisionState.sleepMode.makeDecision(onHumanLost);
  }
}


/*
   Asynchronous thread process
   ===========================
*/
void initDecisions() {
  // Decision thread runs at 5 Hz
  decisionThread.onRun(onDecision);
  decisionThread.setInterval(DECISION_INTERVAL);
}

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
  makeEyesBlinkDecision();
  makeEyesMoveDecision();
  makeSleepDecision();
}

