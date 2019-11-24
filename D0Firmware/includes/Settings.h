
#ifndef __SETTINGS_H
#define __SETTINGS_H

#include "SettingsData.h"

class Settings {
    public:
        config_t config;
        
        void setup();
        void save();
        void reset();
        void printConfig();

        // public for testing
        bool verifyConfig(config_t *config);
        void setDefaultConfig(config_t *config);
        void setV2Defaults(config_t *configPtr);
        

    private:
        

        void writeEeprom(config_t *config);
        void readEeprom(config_t *config);
};

#endif