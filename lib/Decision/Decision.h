#ifndef Decision_h
#define Decision_h

#include <Arduino.h>

struct DECISION {
    uint16_t  lastTime = 0;
    uint8_t   weight = 0;
};

const uint8_t DECISION_MAX_WEIGHT = 255;

class Decision {
    private:
        DECISION state = {};

    public:
        Decision();

        void incrementWeight();
        void setWeight(uint8_t weight);
        void setWeightInRange(uint16_t value, uint16_t minValue, uint16_t maxValue);

        void makeDecision(void callback());
        void resetDecision();
};

#endif