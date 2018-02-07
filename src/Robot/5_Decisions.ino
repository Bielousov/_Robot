#include <Decision.h>

const uint8_t DECISION_INTERVAL = 200;   // Decisions frequency, in ms

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
  if (State.Eyes.isOpened) {
    DecisionState.eyesBlink.incrementWeight();
    DecisionState.eyesBlink.makeDecision(onEyesBlink, true);
  }
};

/*
   Eyes: Move Decisions
   ---------------------------
*/
void makeEyesMoveDecision() {
  DecisionState.eyesMove.incrementWeight();
  DecisionState.eyesMove.makeDecision(onEyesMove, true);
}

/* 
 * I tend to focus more on the point of interest I set in my Environment,
 * however if the decision to move my eyes hasn't been really strong,
 * I may experience some eyes wondering syndrome.
    
 */
void makeEyesWonderDecision() {
  DecisionState.eyesMove.makeOppositeDecision(setRandomPointOfInterest, false);
}


/*
   Global: Sleep / Awakening State
   -------------------------------
*/
const uint8_range sleepModeRange = {30, 255}; // Min / Max time to fall asleep if no humans detected around

void makeSleepDecision() {
  if (isHumanDetected()) {
    DecisionState.sleepMode.setWeightInRange(getTimeSinceHumanDetected(), sleepModeRange.min, sleepModeRange.max);
    DecisionState.sleepMode.makeDecision(onHumanLost, true);
  }
}

/*
 * Decision affecting events
 */
 
void updateDecisionsOnEyesClosed() {
    DecisionState.eyesBlink.resetDecision();
}

void updateDecisionsOnHumanDetection() {
    DecisionState.eyesMove.addWeight(DecisionWeight.hight);
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
  makeEyesWonderDecision();
  makeEyesMoveDecision();
  makeSleepDecision();
}

