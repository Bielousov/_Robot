/*
 * Animation Queue
 * ===============
 */

#include <QList.h>

// Eyes Animation Queue
QList <byte*> animationQueue;

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
