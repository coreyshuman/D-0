
#ifndef __MOTORS_H
#define __MOTORS_H

#include <Adafruit_PWMServoDriver.h>

class Motors {
    public:
        void setup();
        void loop(int test);

    private:
        Adafruit_PWMServoDriver pwm;
};

#endif