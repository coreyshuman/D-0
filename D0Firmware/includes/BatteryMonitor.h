
#ifndef __BATTERYMONITOR_H
#define __BATTERYMONITOR_H

#include <Adafruit_INA219.h>

class BatteryMonitor {
    public:
        float shuntVoltage;
        float busVoltage;
        float loadVoltage;
        float current;
        float power;

        void setup();
        void loop();

    private:
        Adafruit_INA219 ina219;

        enum BatteryMonitorStates {
            BmReadShuntVoltage,
            BmReadBusVoltage,
            BmReadLoadVoltage,
            BmReadCurrent,
            BmReadPower,
            BmStatesCount
        };

        BatteryMonitorStates batteryMonitorState = BmReadShuntVoltage;
};

#endif