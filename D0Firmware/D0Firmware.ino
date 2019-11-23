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


Audio audio;
CommandController controller;
IMU imu;
Motors motors;
Settings settings;


// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
    
    // initialize serial communication
    Serial.begin(115200);
    Serial.println(F("D-0 Firmware Initializing..."));

    // configure LED for output
    pinMode(LED_PIN, OUTPUT);

    settings.setup();
    audio.setup();
    imu.setup(settings.config.imuOffsets);
    motors.setup();
    controller.setup(settings, imu);
}



// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {
    audio.loop(1);
    imu.loop(2);
    controller.loop(1);
    motors.loop(4);
}
