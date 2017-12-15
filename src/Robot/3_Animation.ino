/*
 * Animation Queue
 * ===============
 */

#include <QList.h>

const uint8_t ANIMATION_BUFFER_SIZE = 32;

// Eyes Animation Queue
QList <byte*> animationQueue;
//byte animationBuffer[ANIMATION_BUFFER_SIZE][8];
//
//void initAnimationQueue() {
//    for (uint8_t i = 0; i < ANIMATION_BUFFER_SIZE; i++) {
//      animationBuffer[i] = NULL;
//    }
//}

void addAnimationSequence(
    const byte animationBitmap[][8], 
    const uint8_t* animationSequence, 
    const uint8_t animationSequenceSize
) {
    for (int i = 0; i < animationSequenceSize; i++) {
        byte frameIndex = animationSequence[i];
        byte* frameBitmap = (byte*) animationBitmap[frameIndex];
        pushFrameToAnimationQueue(frameBitmap);
    }
}

void pushFrameToAnimationQueue(byte* frameBitmap) {
    animationQueue.push_back(frameBitmap);
}

byte* popFrameFromAnimationQueue() {
    byte* frameBitmap = animationQueue.front();
    animationQueue.pop_front();
    return frameBitmap;
}

bool isAnimationQueueEmpty() {
    return animationQueue.size() == 0;
}

//byte findEmptyAnimationBuffer() {
//    for (uint8_t i = 0; i < ANIMATION_BUFFER_SIZE - 1; i++) {
//        if(animationBuffer[i] == NULL) {
//            return i;
//        }
//    } 
//
//    return ANIMATION_BUFFER_SIZE - 1;
//}
