
#ifndef __SETTINGS_H
#define __SETTINGS_H

#include "SettingsData.h"

class Settings {
    public:
        settings_t config;
        
        void setup();
        void save();
        void printConfig();

        // public for testing
        bool verifyConfig(settings_t *config);
        void setDefaultConfig(settings_t *config);
        

    private:
        

        void writeEeprom(settings_t *config);
        void readEeprom(settings_t *config);
};

#endif