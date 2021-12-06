#include "Decision.h"

Decision::Decision() {
    state.weight = 0;
}

Decision::Decision(uint8_t initialWeight) {
    state.weight = initialWeight;
}

/*
 * Public methods
 */
void Decision::add(uint8_t weight) {
    if (state.weight > DecisionWeight.max - weight) {
         state.weight = DecisionWeight.max;
    } else {
        state.weight += weight;
    }
}

void Decision::set(uint8_t weight) {
    state.weight = weight;
}

void Decision::subtract(uint8_t weight) {
    if (state.weight < weight) {
         state.weight = DecisionWeight.min;
    } else {
        state.weight -= weight;
    }
}

/*
 * Private methods
 */

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