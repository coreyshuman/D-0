#include "../includes/Audio.h"
#include "MPU6050_6Axis_MotionApps20.h"
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

void Audio::setup() {
    musicPlayer = 
        Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);
    if (! musicPlayer.begin()) { // initialise the music player
       Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
       while (1);
    }
    Serial.println(F("VS1053 found"));
    
     if (!SD.begin(CARDCS)) {
      Serial.println(F("SD failed, or not present"));
      while (1);  // don't do anything more
    }

    musicPlayer.setVolume(20,20);

    // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
    // audio playing
    musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
}

void Audio::loop(int test) {

}