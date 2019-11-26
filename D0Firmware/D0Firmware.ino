// D-0
// Corey Shuman
// 11/13/19
// Arduino-specific API functions, module Initializations, and main loop code goes here.

#include <Arduino.h>
#include "includes/Config.h"
#include "includes/Audio.h"
#include "includes/CommandController.h"
#include "includes/IMU.h"
#include "includes/Motors.h"
#include "includes/SerialProcessor.h"
#include "includes/Settings.h"
#include "includes/BatteryMonitor.h"


Audio audio;
CommandController controller;
IMU imu;
Motors motors;
Settings settings;
BatteryMonitor batteryMonitor;
SerialProcessor serialProcessor;

//cts debug
bool loopPinState = false;


// ================================================================
// ===                    HELPER FUNCTIONS                      ===
// ================================================================

// create non-member function to register callback between classes
// since Arduino doesn't support std::bind
void serialProcessedCallback(char * payload, uint8_t length) {
    controller.addCommand(payload, length);
}

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup() {
    // initialize serial communication
    Serial.begin(115200);
    Serial.println(F("D-0 Firmware Initializing..."));

    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        //TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
        Wire.setClock(400000);
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
    
    // configure LED for output
    pinMode(LED_PIN, OUTPUT);
    // cts debug
    pinMode(53, OUTPUT);

    Serial.println(F("A"));
    settings.setup();
    Serial.println(F("B"));
    audio.setup();
    Serial.println(F("C"));
    imu.setup(settings.config.imuOffsets);
    Serial.println(F("D"));
    motors.setup();
    Serial.println(F("E"));
    batteryMonitor.setup();
    Serial.println(F("F"));
    serialProcessor.setup();
    controller.setup(serialProcessor, settings, imu, batteryMonitor);
    Serial.println(F("G"));
    serialProcessor.registerProcessedCallback(serialProcessedCallback);
    Serial.println(F("H"));
}



// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {
    audio.loop(1);
    imu.loop(2);
    batteryMonitor.loop();
    
    serialProcessor.loop(7);
    controller.loop(1);
    motors.loop(controller.commandState, imu.imuData, settings.config);
    digitalWrite(53, loopPinState);
    loopPinState = !loopPinState;
}

