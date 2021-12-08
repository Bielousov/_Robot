#ifndef Decision_h
#define Decision_h

#include <Arduino.h>

struct DECISION {
    uint32_t  lastTime = 0;
    uint8_t   weight = 0;
};

struct DECISION_WEIGHT {
  uint8_t min = 0;
  uint8_t low = 64;
  uint8_t medium = 128;
  uint8_t high = 196;
  uint8_t max = 255;
};

const DECISION_WEIGHT DecisionWeight;

class Decision {
    private:
        DECISION state = {};
        void _run(void callback(), bool resetDecisionOnSuccess, bool decisionForceCondition);

    public:
        Decision();
        Decision(uint8_t weight);

        void decrement(uint8_t weight);
        void increment(uint8_t weight);
        uint8_t get();
        void set(uint8_t weight);

        void makeDecision(void callback(), bool resetDecisionOnSuccess);
        void makeLowDecision(void callback(), bool resetDecisionOnSuccess);
        void resetDecision();
};

#endif