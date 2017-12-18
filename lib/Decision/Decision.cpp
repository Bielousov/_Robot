#include "Decision.h"

Decision::Decision() {

}

/*
 * Public methods
 */
void Decision::incrementWeight() {
    if (state.weight < DECISION_MAX_WEIGHT) {
        state.weight++;
    }
}

void Decision::setWeight(uint8_t weight) {
    state.weight = weight;
}

void Decision::setWeightInRange(uint16_t value, uint16_t minValue, uint16_t maxValue) {
    uint8_t weight = (value < minValue) ?
        0 : weight = (uint8_t) (DECISION_MAX_WEIGHT * (value - minValue) / (maxValue - minValue));

    setWeight(weight);
}

void Decision::makeDecision(void callback()) {
    bool decision = state.weight > (uint8_t)random(DECISION_MAX_WEIGHT);

    if (!decision) {
        return;
    }

    // Positive decision
    callback();
    resetDecision();
}

void Decision::resetDecision() {
    state.lastTime = millis() / 1000;
    state.weight = 0;
}