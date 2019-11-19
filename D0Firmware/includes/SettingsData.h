#ifndef __SETTINGSDATA_H
#define __SETTINGSDATA_H

#include <stdint.h>

typedef struct __attribute__((packed)) _settings_t
{
    uint16_t    crc;
    uint16_t    length;

    union
    {
        uint8_t asAmpli;
        struct
        {
            uint8_t dir   : 4;
            uint8_t curve : 4;
        };
    } uAmpli;

    union
    {
        uint8_t asForce;
        struct
        {
            uint8_t amount  : 5;
            uint8_t axis    : 3;
        };
    } uForce;
   
}   settings_t;

#define SETTINGS_SIZE sizeof(settings_t)

#endif