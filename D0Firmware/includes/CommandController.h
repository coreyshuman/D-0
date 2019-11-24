
#ifndef __COMMANDCONTROLLER_H
#define __COMMANDCONTROLLER_H

#include "Settings.h"
#include "IMU.h"
#include "BatteryMonitor.h"
#include "CommandData.h"

class CommandController {
    public:
        commands_t commands;
        
        void setup(Settings &settings, IMU &imu, BatteryMonitor &batteryMonitor);
        void loop(int test);

    private:
        Settings *settingsPtr;
        IMU *imuPtr;
        BatteryMonitor *batteryMonitorPtr;
};

#endif