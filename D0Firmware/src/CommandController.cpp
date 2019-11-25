#include <Arduino.h>
#include "../includes/CommandController.h"
#include "../includes/Settings.h"



void CommandController::setup(SerialProcessor &serialProcessor, Settings &settings, IMU &imu, BatteryMonitor &batteryMonitor) {
    settingsPtr = &settings;
    imuPtr = &imu;
    batteryMonitorPtr = &batteryMonitor;
    serialProcessorPtr = &serialProcessor;

    commands.driveSpeed = COMMAND_RANGE_MID;
    commands.turnSpeed = COMMAND_RANGE_MID;
}

void CommandController::loop(int test) {
    // test code
    if (Serial.available() > 0) {
        // read the incoming byte:
        char c = Serial.read();

        switch(c) {
            case 'a':
                commands.flags.armed = !commands.flags.armed;
                Serial.println(commands.flags.armed ? F("Armed") : F("Not Armed"));
                break;
            case 'b': 
                settingsPtr->config.leftDriveServoCal.minimum = Serial.read() - 0x30;
                break;

            case 'i':
                Serial.print("ypr\t");
                Serial.print(imuPtr->imuData.ypr.x * 180/M_PI);
                Serial.print("\t");
                Serial.print(imuPtr->imuData.ypr.y * 180/M_PI);
                Serial.print("\t");
                Serial.println(imuPtr->imuData.ypr.z * 180/M_PI);
                break;
            case 's': 
                settingsPtr->save();
                break; 
            case 'd':
                settingsPtr->reset();   
            case 'c': 
                Serial.println("Calibrating...");
                settingsPtr->config.imuOffsets = imuPtr->calibrate(); 
                settingsPtr->save();
                Serial.println("Done.");
                break;
            case 'p': settingsPtr->printConfig(); break;
            case 'w':
                serialProcessorPtr->writeString("This is a test string.");
                break;
            case 'v':
                Serial.print("Bus Voltage:   "); Serial.print(batteryMonitorPtr->busVoltage); Serial.println(" V");
                Serial.print("Shunt Voltage: "); Serial.print(batteryMonitorPtr->shuntVoltage); Serial.println(" mV");
                Serial.print("Load Voltage:  "); Serial.print(batteryMonitorPtr->loadVoltage); Serial.println(" V");
                Serial.print("Current:       "); Serial.print(batteryMonitorPtr->current); Serial.println(" mA");
                Serial.print("Power:         "); Serial.print(batteryMonitorPtr->power); Serial.println(" mW");
                Serial.println("");
                break;
        }
    }
}