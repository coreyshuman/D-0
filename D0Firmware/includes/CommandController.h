
#ifndef __COMMANDCONTROLLER_H
#define __COMMANDCONTROLLER_H

#include "Settings.h"
#include "IMU.h"
#include "BatteryMonitor.h"
#include "CommandData.h"
#include "SerialProcessor.h"

typedef struct _command_table_t
{
    char commandCode[2];
    uint8_t paramsMin;
    uint8_t paramsMax;
    void (*commandHandler)();

} command_table_t;

class CommandController {
    public:
        command_state_t commandState;
        
        void setup(SerialProcessor &serialProcessor, Settings &settings, IMU &imu, BatteryMonitor &batteryMonitor);
        void loop(int test);
        void addCommand(char *, uint8_t length);
        void processCommand(command_t command);

    private:
        Settings *settingsPtr;
        IMU *imuPtr;
        BatteryMonitor *batteryMonitorPtr;
        SerialProcessor *serialProcessorPtr;
        command_t commandBuffer[8];
        uint8_t commandBufferCount;
        uint8_t commandBufferWriteIndex;
        uint8_t commandBufferReadIndex;

        
};

#endif