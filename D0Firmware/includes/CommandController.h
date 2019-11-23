
#ifndef __COMMANDCONTROLLER_H
#define __COMMANDCONTROLLER_H

#include "Settings.h"
#include "IMU.h"

class CommandController {
    public:
        void setup(Settings settings, IMU imu);
        void loop(int test);

    private:
        Settings *settingsPtr;
        IMU *imuPtr;
};

#endif