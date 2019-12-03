
#ifndef __UTIL_H
#define __UTIL_H

class Util {
    public:
        static uint16_t calculateCrc(uint8_t *data, uint32_t len);
        static void printHex(uint8_t val);
        static int16_t atoi(const char *str);
        static float atof(const char *str);
        static void itoh(uint16_t val, char* returnStringDestination);
        static uint16_t htoi(const char *str);

    private:
};

#endif