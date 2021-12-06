#ifndef Dictionary_h
#define Dictionary_h

#include <Arduino.h>
#include <avr/pgmspace.h>

static char strBuffer[32];

class Dictionary {
    private:
        char Buffer[32];

    public:
        Dictionary();

        // Methods
        char* get(const char* str);
};

#endif