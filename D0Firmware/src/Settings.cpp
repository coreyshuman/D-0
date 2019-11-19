#include "../includes/Settings.h"
#include "../includes/Util.h"



void Settings::setup() {
    // todo: read config from eeprom
    if(!VerifyConfig(&config)) {
        SetDefaultConfig(&config);
    }
}

bool Settings::VerifyConfig(settings_t *config)
{
    if(config == nullptr) return false;
    if(config->length > 256 || config->length < 4) return false;

    uint16_t crc = Util::CalculateCrc(((uint8_t *)config)+2, config->length - 2);
    if(crc == config->crc && config->length == SETTINGS_SIZE) {
        // future - check length for possible config updates
        return true;
    } 

    return false;
}

void Settings::SetDefaultConfig(settings_t *config)
{
    if(config == nullptr) throw "Config can not be null";

    config->length = SETTINGS_SIZE;
    // plug in defaults...

    config->crc = Util::CalculateCrc(((uint8_t *)config)+2, SETTINGS_SIZE-2);
}