#ifndef __SETTINGSDATA_H
#define __SETTINGSDATA_H

#include <stdint.h>

typedef struct __attribute__((packed)) _imu_offsets_t
{
    uint16_t    xAccelOffset;
    uint16_t    yAccelOffset;
    uint16_t    zAccelOffset;
    uint16_t    xGyroOffset;
    uint16_t    yGyroOffset;
    uint16_t    zGyroOffset;
    union
    {
        uint8_t map;
        struct
        {
            uint8_t valid   : 1;
            uint8_t unused : 7;
        };
    } flags;

    uint8_t unused[3];
}   imu_offsets_t;

typedef struct __attribute__((packed)) _free_animation_t
{
    float    stabilizationP;
    float    stabilizationI;
    float    stabilizationD;
    float    neckLeanP;
    float    neckLeanI;
    float    neckLeanD;
    float    headTurnP;
    float    headTurnI;
    float    headTurnD;

    uint32_t    unused1[7];
    
    union
    {
        uint8_t map;
        struct
        {
            uint8_t stabilizationEnabled    : 1;
            uint8_t neckLeanEnabled         : 1;
            uint8_t headTurnEnabled         : 1;
            uint8_t unused                  : 5;
        };
    } flags;

    uint8_t unused2[3];
}   free_animation_t;

typedef struct __attribute__((packed)) _servo_calibration_t
{
    uint16_t minimum;
    uint16_t center;
    uint16_t maximum;
    uint16_t deadband;

    uint8_t unused[8];
} servo_calibration_t;

typedef struct __attribute__((packed)) _config_t
{
    uint16_t    crc;
    uint16_t    length;
    uint16_t    version;
    uint16_t    unused1;

    imu_offsets_t imuOffsets;
    servo_calibration_t leftDriveServoCal;
    servo_calibration_t rightDriveServoCal;
    servo_calibration_t neckAngleServoCal;
    servo_calibration_t neckLeanServoCal;
    servo_calibration_t headTurnServoCal;
    servo_calibration_t headTiltServoCal;
    free_animation_t freeAnimation;

}   config_t;

#define SETTINGS_SIZE sizeof(config_t)

#endif