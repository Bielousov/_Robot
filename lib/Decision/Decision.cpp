#include "Decision.h"

Decision::Decision() {

}

/*
 * Public methods
 */
void Decision::addWeight(uint8_t weight) {
    if (state.weight + weight > DecisionWeight.max) {
        // Weigh overflow
        state.weight = DecisionWeight.max;
    } else {
        state.weight += weight;
    }
}

void Decision::incrementWeight() {
    if (state.weight < DecisionWeight.max) {
        state.weight++;
    }
}

void Decision::setWeight(uint8_t weight) {
    state.weight = weight;
}

void Decision::setWeightInRange(uint16_t value, uint16_t minValue, uint16_t maxValue) {
    uint8_t weight = (value < minValue) ?
        0 : weight = (uint8_t) (DecisionWeight.max * (value - minValue) / (maxValue - minValue));

    setWeight(weight);
}

void Decision::_makeDecision(void callback(), bool resetDecisionOnSuccess, bool decisionCondition) {
    if (decisionCondition) {
        callback();

        if (resetDecisionOnSuccess) {
            resetDecision();
        }
    }
}

// Execute callback if weight is high
void Decision::makeDecision(void callback(), bool resetDecisionOnSuccess) {
    bool decisionCondition = state.weight > (uint8_t)random(DecisionWeight.max);
    _makeDecision(callback, resetDecisionOnSuccess, decisionCondition);
}

// Execute callback if weight is low
void Decision::makeOppositeDecision(void callback(), bool resetDecisionOnSuccess) {
    bool decisionCondition = state.weight < (uint8_t)random(DecisionWeight.max);
    _makeDecision(callback, resetDecisionOnSuccess, decisionCondition);
}

void Decision::resetDecision() {
    state.lastTime = millis() / 1000;
    state.weight = 0;
}