#include <Decision.h>

const uint8_t DECISION_INTERVAL = 50;   // Decisions frequency, in ms

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
    DecisionState.eyesBlink.addWeight(1000 / DECISION_INTERVAL);
    DecisionState.eyesBlink.makeDecision(onEyesBlink, true);
  }
};

/*
   Eyes: Move Decisions
   ---------------------------
*/
void makeEyesMoveDecision() {
  DecisionState.eyesMove.addWeight(1000 / DECISION_INTERVAL);
  DecisionState.eyesMove.makeDecision(onEyesMove, true);
}

/* 
 * I tend to focus more on the focus point,
 * however if the decision to move my eyes hasn't been really strong,
 * I may experience some eyes wondering syndrome.
    
 */
void makeEyesWonderDecision() {
  DecisionState.eyesMove.makeOppositeDecision(setRandomFocus, false);
}


/*
 * Decision affecting events
 */
 
void updateDecisionsOnEyesClosed() {
    DecisionState.eyesBlink.resetDecision();
}

/*
   Asynchronous thread process
   ===========================
*/
void initDecisions() {
  // Decision thread runs at 20 Hz
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
}
