// include stack library header.
#include <QList.h>

// Eyes State
QList <byte*> eyesAnimationQueue;

void addAnimationSequence(const byte animationBitmap[][8], const uint8_t animationSequence[], const uint8_t animationSequenceSize) {
    for (int i = 0; i < animationSequenceSize; i++) {
        byte frameIndex = animationSequence[i];
        byte* frameBitmap = (byte*) animationBitmap[frameIndex];
        pushFrameToAnimationQueue(frameBitmap);
    }
}

void pushFrameToAnimationQueue(byte frameBitmap[8]) {
    eyesAnimationQueue.push_back(frameBitmap);
}

byte* popFrameFromAnimationQueue() {
    byte* frameBitmap = eyesAnimationQueue.front();
    eyesAnimationQueue.pop_front();
    return frameBitmap;
}

bool isAnimationQueueEmpty() {
    return eyesAnimationQueue.size() == 0;
}
