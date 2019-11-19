
#ifndef __SETTINGS_H
#define __SETTINGS_H

#include "SettingsData.h"

class Settings {
    public:
        void setup();
        bool VerifyConfig(settings_t *config);
        void SetDefaultConfig(settings_t *config);

    private:
        settings_t config;
};

#endif