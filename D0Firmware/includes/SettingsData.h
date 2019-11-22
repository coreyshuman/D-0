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
}   imu_offsets_t;

typedef struct __attribute__((packed)) _servo_calibration_t
{
    uint16_t minimum;
    uint16_t center;
    uint16_t maximum;
    uint16_t deadband;
} servo_calibration_t;

typedef struct __attribute__((packed)) _settings_t
{
    uint16_t    crc;
    uint16_t    length;

    imu_offsets_t imuOffsets;
    servo_calibration_t leftDriveServoCal;
    servo_calibration_t rightDriveServoCal;
    servo_calibration_t neckAngleServoCal;
    servo_calibration_t neckLeanServoCal;
    servo_calibration_t headTurnServoCal;
    servo_calibration_t headTiltServoCal;

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