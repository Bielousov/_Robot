#ifndef Decision_h
#define Decision_h

#include <Arduino.h>

struct DECISION {
    long    lastTime = 0;
    uint8_t weight = 0;
};

struct DECISION_WEIGHT {
  uint8_t min = 0;
  uint8_t small = 64;
  uint8_t medium = 128;
  uint8_t high = 196;
  uint8_t max = 255;
};

const DECISION_WEIGHT DecisionWeight;

class Decision {
    private:
        DECISION state = {};

        void _makeDecision(void callback(), bool resetDecisionOnSuccess, bool decision);

    public:
        Decision();
        Decision(uint8_t initialWeight);

        void add(uint8_t weight);
        void set(uint8_t weight);
        void subtract(uint8_t weight);

        void makeDecision(void callback(), bool resetDecisionOnSuccess);
        void makeOppositeDecision(void callback(), bool resetDecisionOnSuccess);
        void resetDecision();
};

#endif