
#ifndef __COMMANDCONTROLLER_H
#define __COMMANDCONTROLLER_H

#include "Settings.h"
#include "IMU.h"
#include "BatteryMonitor.h"
#include "CommandData.h"
#include "SerialProcessor.h"

class CommandController {
    public:
        command_state_t commands;
        
        void setup(SerialProcessor &serialProcessor, Settings &settings, IMU &imu, BatteryMonitor &batteryMonitor);
        void loop(int test);

    private:
        Settings *settingsPtr;
        IMU *imuPtr;
        BatteryMonitor *batteryMonitorPtr;
        SerialProcessor *serialProcessorPtr;
};

#endif