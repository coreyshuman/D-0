#include <Arduino.h>
#include "../includes/Settings.h"
#include "../includes/Util.h"



void Settings::setup() {
    // todo: read config from eeprom
    if(!verifyConfig(&config)) {
        readEeprom(&config);
        setDefaultConfig(&config);
    }
}

void Settings::save() {
    config.crc = Util::calculateCrc(((uint8_t *)config)+2, SETTINGS_SIZE-2);
    writeEeprom(&config);
}

bool Settings::verifyConfig(settings_t *configPtr)
{
    if(configPtr == nullptr) return false;
    if(configPtr->length > 256 || configPtr->length < 4) return false;

    uint16_t crc = Util::calculateCrc(((uint8_t *)configPtr)+2, configPtr->length - 2);
    if(crc == configPtr->crc && configPtr->length == SETTINGS_SIZE) {
        // future - check length for possible config updates
        return true;
    } 

    return false;
}

void Settings::setDefaultConfig(settings_t *configPtr)
{
    if(configPtr == nullptr) throw "Config can not be null";

    configPtr->length = SETTINGS_SIZE;
    // plug in defaults...
    configPtr->leftDriveServoCal.min = 150;
    configPtr->leftDriveServoCal.center = 375;
    configPtr->leftDriveServoCal.max = 600;
    configPtr->leftDriveServoCal.deadband = 50;
    configPtr->rightDriveServoCal.min = 150;
    configPtr->rightDriveServoCal.center = 375;
    configPtr->rightDriveServoCal.max = 600;
    configPtr->rightDriveServoCal.deadband = 50;
    configPtr->neckAngleServoCal.min = 150;
    configPtr->neckAngleServoCal.center = 375;
    configPtr->neckAngleServoCal.max = 600;
    configPtr->neckAngleServoCal.deadband = 50;
    configPtr->neckLeanServoCal.min = 150;
    configPtr->neckLeanServoCal.center = 375;
    configPtr->neckLeanServoCal.max = 600;
    configPtr->neckLeanServoCal.deadband = 50;
    configPtr->headTurnServoCal.min = 150;
    configPtr->headTurnServoCal.center = 375;
    configPtr->headTurnServoCal.max = 600;
    configPtr->headTurnServoCal.deadband = 50;
    configPtr->headTiltServoCal.min = 150;
    configPtr->headTiltServoCal.center = 375;
    configPtr->headTiltServoCal.max = 600;
    configPtr->headTiltServoCal.deadband = 50;
    

    configPtr->crc = Util::calculateCrc(((uint8_t *)configPtr)+2, SETTINGS_SIZE-2);
}

void Settings::printConfig() {
    uint8_t *data = config;
    int i;
    
    for(i=0; i<SETTINGS_SIZE; i++) {
        if(i%16 == 0) Serial.print("\n");
        Serial.print(*data++, HEX);
    }
    Serial.print("\n");
}

void Settings::writeEeprom(settings_t *configPtr) {
    uint8_t *data = configPtr;
    int i;
    
    for(i=0; i<SETTINGS_SIZE; i++) {
        EEPROM.write(i++, *data++);
    }
}

void Settings::readEeprom(settings_t *configPtr) {
    uint8_t *data = configPtr;
    int i;
    
    for(i=0; i<SETTINGS_SIZE; i++) {
        *data++ = EEPROM.read(i++);
    }
}