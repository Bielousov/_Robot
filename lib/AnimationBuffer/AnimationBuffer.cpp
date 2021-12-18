#include "AnimationBuffer.h"

AnimationBuffer::AnimationBuffer() {
}

byte* AnimationBuffer::addFrame(byte* frameBitmap) {
    return addFrame(frameBitmap, 0);
}

byte* AnimationBuffer::addFrame(byte* frameBitmap, uint8_t frames) {
    if (animationPointer < ANIMATION_BUFFER_SIZE - 1) {
        animationPointer++;
    }

    memcpy(animationQueue[animationPointer], frameBitmap, ANIMATION_BITMAP_SIZE);
    animationMeta[animationPointer] = frames;
    return animationQueue[animationPointer];
}

byte* AnimationBuffer::popFrame() {
    byte* frameBitmap = animationQueue[0];
    
    if (animationMeta[0] > 0) {
        animationMeta[0]--;
    } else {
        // Shift queue array
        if (animationPointer > 0) {
            memcpy(&animationMeta[0], &animationMeta[1], sizeof(animationMeta[0]) * animationPointer);
            memcpy(animationQueue[0], animationQueue[1], sizeof(animationQueue[0]) * animationPointer);
        }
        animationPointer--;
    }

    return frameBitmap;
}

bool AnimationBuffer::isQueueEmpty() {
    return animationPointer < 0;
}
