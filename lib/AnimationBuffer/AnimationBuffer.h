#ifndef AnimationBuffer_h
#define AnimationBuffer_h

#include <Arduino.h>
#include <QList.h>

class AnimationBuffer {
    const static uint8_t ANIMATION_BITMAP_SIZE = 8;
    const static uint8_t ANIMATION_BUFFER_SIZE = 16;

    private:
        byte animationBuffer[ANIMATION_BUFFER_SIZE][ANIMATION_BITMAP_SIZE];
        uint8_t animationBufferPointer = 0;

        // Eyes Animation Queue
        QList <byte*> animationQueue;

    public:
        AnimationBuffer();

        void addFrame(byte* frameBitmap);
        byte* getFrame();
        byte* getFrameBuffer();
        bool isQueueEmpty();
};

#endif