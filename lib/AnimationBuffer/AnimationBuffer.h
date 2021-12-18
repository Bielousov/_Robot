#ifndef AnimationBuffer_h
#define AnimationBuffer_h

#include <Arduino.h>

class AnimationBuffer {
    const static uint8_t ANIMATION_BITMAP_SIZE = 8;
    const static uint8_t ANIMATION_BUFFER_SIZE = 16;

    private:
        int8_t animationPointer = -1;
        uint8_t animationMeta[ANIMATION_BUFFER_SIZE];
        byte animationQueue[ANIMATION_BUFFER_SIZE][ANIMATION_BITMAP_SIZE];
    public:
        AnimationBuffer();

        byte* addFrame(byte* frameBitmap);
        byte* addFrame(byte* frameBitmap, uint8_t frames);
        byte* popFrame();
        bool isQueueEmpty();
};

#endif