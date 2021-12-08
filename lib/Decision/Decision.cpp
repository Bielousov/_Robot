#include "Decision.h"

Decision::Decision() {
    state.lastTime = 0;
    state.weight = 0;
}

Decision::Decision(uint8_t weight) {
    state.lastTime = 0;
    state.weight = weight;
};

/*
 * Private methods
 */

void Decision::_run(void callback(), bool resetDecisionOnSuccess, bool decisionForceCondition) {
    if (decisionForceCondition) {
        callback();

        if (resetDecisionOnSuccess) {
            resetDecision();
        }
    }
}

/*
 * Public methods
 */

void Decision::decrement(uint8_t weight) {
    if (state.weight < weight) {
        state.weight = DecisionWeight.min;
    } else {
        state.weight -= weight;
    }
}

void Decision::increment(uint8_t weight) {
    if (state.weight > DecisionWeight.max - weight) {
        state.weight = DecisionWeight.max;
    } else {
        state.weight += weight;
    }
}

uint8_t Decision::get() {
    return state.weight;
}

void Decision::set(uint8_t weight) {
    state.weight = weight;
}

// Execute callback if weight is high
void Decision::makeDecision(void callback(), bool resetDecisionOnSuccess) {
    uint16_t dice = (uint8_t)random(DecisionWeight.min, DecisionWeight.max);
    bool decisionCondition = state.weight > dice;
    _run(callback, resetDecisionOnSuccess, decisionCondition);
}

// Execute callback if weight is not high
void Decision::makeLowDecision(void callback(), bool resetDecisionOnSuccess) {
    bool decisionCondition = state.weight < (uint8_t)random(DecisionWeight.low, DecisionWeight.high);
    _run(callback, resetDecisionOnSuccess, decisionCondition);
}

void Decision::resetDecision() {
    state.lastTime = millis() / 1000;
    state.weight = 0;
}