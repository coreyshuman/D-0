#ifndef __PLATFORM_H
#define __PLATFORM_H

#define SERVO_FREQ 60 // Analog servos run at ~60 Hz updates

// Configure MPU6050 I2C Address
// AD0 low = 0x68 (default for SparkFun breakout and InvenSense evaluation board)
// AD0 high = 0x69
#define MPU_I2C_ADDRESS 0x68

// Define hardware platform (pins and peripherals)
#define PLATFORM_ARDUINO_UNO


#include <Wire.h>
#include "I2Cdev.h"
// include platform definitions
#include "platform/Platform.h"


#endif
