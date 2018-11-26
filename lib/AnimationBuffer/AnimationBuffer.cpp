#include "AnimationBuffer.h"

AnimationBuffer::AnimationBuffer() {

}

void AnimationBuffer::addFrame(byte* frameBitmap) {
    animationQueue.push_back(frameBitmap);
}

byte* AnimationBuffer::getFrame() {
    byte* frameBitmap = animationQueue.front();
    animationQueue.pop_front();
    return frameBitmap;
}

byte* AnimationBuffer::getFrameBuffer() {
    if (isQueueEmpty()) {
       animationBufferPointer = 1;
    } else if (animationBufferPointer >= ANIMATION_BUFFER_SIZE - 1) { 
         animationBufferPointer = ANIMATION_BUFFER_SIZE;
    } else {
       animationBufferPointer++;
    }

    return animationBuffer[animationBufferPointer - 1];
}

bool AnimationBuffer::isQueueEmpty() {
    return animationQueue.size() == 0;
}