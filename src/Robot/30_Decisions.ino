#include <Decision.h>

const uint16_t DECISION_INTERVAL = 250;   // Decision making frequency, in ms
const uint8_t EYES_BLINK_WEIGHT = 3;
const uint8_t EYES_MOVE_WEIGHT = 1;
const uint8_t SAY_HELLO_WEIGHT = 128;

struct DECISION_STATE {
  Decision eyesBlink = Decision(0);
  Decision eyesMove = Decision(0);
  Decision sleepMode = Decision(0);
  Decision sayHello = Decision(128);
};

DECISION_STATE DecisionState;

// Decisions thread
Thread decisionThread = Thread(onDecision, DECISION_INTERVAL);

/*
   Eyes: Blink Decisions
   ---------------------
*/
void makeEyesBlinkDecision() {
  if (State.Eyes.isOpened) {
    DecisionState.eyesBlink.add(EYES_BLINK_WEIGHT);
    DecisionState.eyesBlink.makeDecision(blinkEyes, true);
  }
};

/*
   Eyes: Move Decisions
   ---------------------------
*/
void makeEyesMoveDecision() {
  DecisionState.eyesMove.add(EYES_MOVE_WEIGHT);
  DecisionState.eyesMove.makeDecision(moveEyes, true);
}

/* 
 * I tend to focus more on the focus point,
 * however if the decision to move my eyes hasn't been really strong,
 * I may experience some eyes wondering syndrome.
    
 */
void makeEyesWonderDecision() {
  DecisionState.eyesMove.makeLowDecision(setRandomPointOfInterest, false);
}


/*
 * Decision affecting events
 */
 
void updateDecisionsOnEyesClosed() {
    DecisionState.eyesBlink.resetDecision();
}


/*
   Voice: Say Hello Decision
   -------------------------
*/

void makeSayHelloDecision() {
  DecisionState.sayHello.subtract(SAY_HELLO_WEIGHT);
  DecisionState.sayHello.makeDecision(sayHello, true);
}

/*
   Thread callback
   ---------------
*/
void onDecision() {
  makeEyesBlinkDecision();
  makeEyesWonderDecision();
  makeEyesMoveDecision();
  makeSayHelloDecision();
}

/*
   Asynchronous thread process
   ===========================
*/

void runDecisionsThread() {
  if (decisionThread.shouldRun()) {
     decisionThread.run();
     freeMemoryLog();
  }
}
