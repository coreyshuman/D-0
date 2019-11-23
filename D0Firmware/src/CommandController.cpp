#include <Arduino.h>
#include "../includes/CommandController.h"
#include "../includes/Settings.h"



void CommandController::setup(Settings settings, IMU imu) {
    settingsPtr = &settings;
    imuPtr = &imu;
}

void CommandController::loop(int test) {
    // test code
    if (Serial.available() > 0) {
        // read the incoming byte:
        char c = Serial.read();

        switch(c) {
            case 'a': 
                settingsPtr->config.leftDriveServoCal.minimum = Serial.read() - 0x30;
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
        }
    }
}