// Mock Arduino.h file for unit testing.
#ifndef __ARDUINO_MOCK_H
#define __ARDUINO_MOCK_H

#include <stdint.h>
#include <stdio.h> // for size_t

typedef uint8_t byte;

#define DEC 10
#define HEX 16
#define OCT 8
#ifdef BIN // Prevent warnings if BIN is previously defined in "iotnx4.h" or similar
#undef BIN
#endif
#define BIN 2



class Serial_ {
    public:
        static size_t print(const char *str) { };
        static size_t print(int val, int format) { };
        static size_t println(const char *str) { };

    private:
        static char serialBufferTx[1024];
        static int serialBufferTxIdx;
        static int serialBufferTxLen;
};

class EEPROM_ {
    public:
        static uint16_t length() { }
        static uint8_t read(int idx) { }
        static void write(int idx, uint8_t val) { }
};

extern Serial_ Serial;
extern EEPROM_ EEPROM;

#endif