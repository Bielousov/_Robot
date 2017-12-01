// include stack library header.
#include <QList.h>

// Eyes State
QList <byte*> eyesAnimationQueue;

void addAnimationSequence(const byte animationBitmap[][8], const byte animationSequence[], const byte animationSequenceSize) {
  for (int i = 0; i < animationSequenceSize; i++) {
    byte frameIndex = animationSequence[i];
    byte* frameBitmap = (byte*) animationBitmap[frameIndex];
    pushFrameToAnimationQueuek(frameBitmap);
  }
}

void pushFrameToAnimationQueuek(byte frameBitmap[8]) {
  eyesAnimationQueue.push_back(frameBitmap);
}

byte* popFrameFromAnimationQueue() {
  byte* frameBitmap = eyesAnimationQueue.front();
  eyesAnimationQueue.pop_front();
  return frameBitmap;
}
