/*
 * Animation Queue
 * ===============
 */

#include <QList.h>

const uint8_t ANIMATION_BUFFER_SIZE = 16;
byte animationBuffer[ANIMATION_BUFFER_SIZE][8];
byte animationBufferCurrentPosition = 0;

// Eyes Animation Queue
QList <byte*> animationQueue;

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

byte* findEmptyAnimationBuffer() {
    if (isAnimationQueueEmpty() || animationBufferCurrentPosition >= ANIMATION_BUFFER_SIZE - 1) {
        animationBufferCurrentPosition = 0;
        return animationBuffer[animationBufferCurrentPosition];
    } else {
        animationBufferCurrentPosition++;
        return animationBuffer[animationBufferCurrentPosition - 1];
    }
}
