#include <Arduino.h>
#include <EEPROM.h>
#include "../includes/Settings.h"
#include "../includes/Util.h"


void Settings::setup() {
    readEeprom(&config);
    if(!verifyConfig(&config)) {
        Serial.println("Using default config.");
        setDefaultConfig(&config);
    }
}

void Settings::save() {
    config.crc = Util::calculateCrc(((uint8_t *)&config)+2, SETTINGS_SIZE-2);
    writeEeprom(&config);
}

void Settings::reset() {
    setDefaultConfig(&config);
}

bool Settings::verifyConfig(settings_t *configPtr)
{
    if(configPtr == nullptr) return false;
    if(configPtr->length > EEPROM.length() || configPtr->length < 4) return false;

    uint16_t crc = Util::calculateCrc(((uint8_t *)configPtr)+2, configPtr->length - 2);
    if(crc == configPtr->crc && configPtr->length <= SETTINGS_SIZE) {
        
        if(configPtr->version < 2) {
            // future - check for possible config updates
            // applyV2Defaults();
        }
        return true;
    } 

    return false;
}

void Settings::setDefaultConfig(settings_t *configPtr)
{
    int i;
    if(configPtr == nullptr) {
        configPtr = new settings_t();
    }

    uint8_t *tmpPtr = (uint8_t *)configPtr;
    for(i=0; i<SETTINGS_SIZE; i++) {
        *tmpPtr++ = 0;
    }

    configPtr->length = SETTINGS_SIZE;
    configPtr->version = 1;
    // plug in defaults...
    configPtr->leftDriveServoCal.minimum = 150;
    configPtr->leftDriveServoCal.center = 375;
    configPtr->leftDriveServoCal.maximum = 600;
    configPtr->leftDriveServoCal.deadband = 50;
    configPtr->rightDriveServoCal.minimum = 150;
    configPtr->rightDriveServoCal.center = 375;
    configPtr->rightDriveServoCal.maximum = 600;
    configPtr->rightDriveServoCal.deadband = 50;
    configPtr->neckAngleServoCal.minimum = 150;
    configPtr->neckAngleServoCal.center = 375;
    configPtr->neckAngleServoCal.maximum = 600;
    configPtr->neckAngleServoCal.deadband = 50;
    configPtr->neckLeanServoCal.minimum = 150;
    configPtr->neckLeanServoCal.center = 375;
    configPtr->neckLeanServoCal.maximum = 600;
    configPtr->neckLeanServoCal.deadband = 50;
    configPtr->headTurnServoCal.minimum = 150;
    configPtr->headTurnServoCal.center = 375;
    configPtr->headTurnServoCal.maximum = 600;
    configPtr->headTurnServoCal.deadband = 50;
    configPtr->headTiltServoCal.minimum = 150;
    configPtr->headTiltServoCal.center = 375;
    configPtr->headTiltServoCal.maximum = 600;
    configPtr->headTiltServoCal.deadband = 50;
    

    configPtr->crc = Util::calculateCrc(((uint8_t *)configPtr)+2, SETTINGS_SIZE-2);
}

void Settings::printConfig() {
    uint8_t *data = (uint8_t *)&config;
    int i;
    
    for(i=0; i<SETTINGS_SIZE; i++) {
        if(i%16 == 0) Serial.print("\n");
        Util::printHex(*data++);
        Serial.print(" ");
    }
    Serial.print("\n");
}

void Settings::writeEeprom(settings_t *configPtr) {
    uint8_t *data = (uint8_t *)configPtr;
    int i;
    
    for(i=0; i<SETTINGS_SIZE; ) {
        EEPROM.write(i++, *data++);
    }
}

void Settings::readEeprom(settings_t *configPtr) {
    uint8_t *data = (uint8_t *)configPtr;
    int i;
    
    for(i=0; i<SETTINGS_SIZE; ) {
        *data++ = EEPROM.read(i++);
    }
}