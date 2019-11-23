#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "../includes/Audio.h"
#include "../includes/Config.h"

Adafruit_VS1053_FilePlayer audioPlayer = 
        Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);

void Audio::setup() {
    
    if (!SD.begin(CARDCS)) {
        Serial.println(F("SD failed, or not present"));
        return;
    }

    if (! audioPlayer.begin()) { // initialise the music player
        Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
        return;
    }
    Serial.println(F("VS1053 found"));
    
    

    audioPlayer.setVolume(20,20);

    // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
    // audio playing
    audioPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
}

void Audio::loop(int test) {

}