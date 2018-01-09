#ifndef Decision_h
#define Decision_h

#include <Arduino.h>

struct DECISION {
    uint16_t  lastTime = 0;
    uint8_t   weight = 0;
};

struct DECISION_WEIGHT {
  uint8_t min = 0;
  uint8_t small = 16;
  uint8_t medium = 32;
  uint8_t hight = 64;
  uint8_t max = 255;
};

const DECISION_WEIGHT DecisionWeight;

class Decision {
    private:
        DECISION state = {};

        void _makeDecision(void callback(), bool resetDecisionOnSuccess, bool decision);

    public:
        Decision();

        void addWeight(uint8_t weight);
        void incrementWeight();
        void setWeight(uint8_t weight);
        void setWeightInRange(uint16_t value, uint16_t minValue, uint16_t maxValue);

        void makeDecision(void callback(), bool resetDecisionOnSuccess);
        void makeOppositeDecision(void callback(), bool resetDecisionOnSuccess);
        void resetDecision();
};

#endif