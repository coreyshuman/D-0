#include "../includes/BatteryMonitor.h"
#include "../includes/Config.h"
#include "../includes/platform/Platform.h"

void BatteryMonitor::setup() {
    ina219 = Adafruit_INA219(INA219_I2C_ADDRESS);
    ina219.begin();
    // begin will reset the Wire speed, so fix that here
    Wire.setClock(400000);
    //ina219.setCalibration_32V_1A();
}

void BatteryMonitor::loop() {
    switch(batteryMonitorState) {
        case BmReadShuntVoltage:
            shuntVoltage = ina219.getShuntVoltage_mV();
            batteryMonitorState = BmReadBusVoltage;
            //Serial.print("Shunt Voltage: "); Serial.print(shuntVoltage); Serial.println(" mV");
            break;
        case BmReadBusVoltage:
            busVoltage = ina219.getBusVoltage_V();
            batteryMonitorState = BmReadLoadVoltage;
            //Serial.print("Bus Voltage:  "); Serial.print(busVoltage); Serial.println(" V");
            break;
        case BmReadLoadVoltage:
            loadVoltage = busVoltage + (shuntVoltage / 1000);
            batteryMonitorState = BmReadCurrent;
            break;
        case BmReadCurrent:
            current = ina219.getCurrent_mA();
            batteryMonitorState = BmReadPower;
            break;
        case BmReadPower:
            power = ina219.getPower_mW();
            batteryMonitorState = BmReadShuntVoltage;
            break;
    }

}
