
#ifndef __UTIL_H
#define __UTIL_H

class Util {
    public:
        static uint16_t calculateCrc(uint8_t *data, uint32_t len);
        static void printHex(byte val);
        static int16_t atoi(const char *str);
        static float atof(const char *str);

    private:
};

#endif