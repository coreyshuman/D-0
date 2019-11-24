
#ifndef __MOTORS_H
#define __MOTORS_H

#include <Adafruit_PWMServoDriver.h>
#include "CommandData.h"
#include "SettingsData.h"
#include "IMUData.h"

#define DRIVE_MOTOR_LEFT    0
#define DRIVE_MOTOR_RIGHT   1

class Motors {
    public:
        void setup();
        void loop(commands_t &commands, imu_data_t &imuData, config_t &config);

    private:
        Adafruit_PWMServoDriver pwm;
};

#endif