// D-0
// Corey Shuman
// 11/13/19
// Arduino-specific API functions, module Initializations, and main loop code goes here.

#include <Arduino.h>
#include "includes/Audio.h"
#include "includes/CommandController.h"
#include "includes/IMU.h"
#include "includes/Motors.h"
#include "includes/SerialProcessor.h"
#include "includes/Settings.h"

#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)

Audio audio;
CommandController controller;
IMU imu;
Motors motors;
Settings settings;


// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup() {
    // initialize serial communication
    Serial.begin(115200);
    Serial.println(F("D-0 Firmware Initializing..."));

    // configure LED for output
    pinMode(LED_PIN, OUTPUT);

    settings.setup();
    audio.setup();
    imu.setup();
    motors.setup();
    controller.setup();
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
