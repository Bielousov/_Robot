#include "Decision.h"

Decision::Decision() {

}

/*
 * Public methods
 */
void Decision::addWeight(uint16_t weight) {
    if (state.weight + weight > DecisionWeight.max) {
        // Weigh overflow
        state.weight = DecisionWeight.max;
    } else {
        state.weight += weight;
    }
}

void Decision::setWeight(uint16_t weight) {
    state.weight = weight;
}

void Decision::setWeightInRange(uint16_t value, uint16_t minValue, uint16_t maxValue) {
    uint16_t weight = (value < minValue) 
        ? 0 
        : (uint16_t) (DecisionWeight.max * (value - minValue) / (maxValue - minValue));

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
    bool decisionCondition = state.weight > (uint16_t)random(DecisionWeight.max);
    _makeDecision(callback, resetDecisionOnSuccess, decisionCondition);
}

// Execute callback if weight is low
void Decision::makeOppositeDecision(void callback(), bool resetDecisionOnSuccess) {
    bool decisionCondition = state.weight < (uint16_t)random(DecisionWeight.max);
    _makeDecision(callback, resetDecisionOnSuccess, decisionCondition);
}

void Decision::resetDecision() {
    state.lastTime = millis() / 1000;
    state.weight = 0;
}