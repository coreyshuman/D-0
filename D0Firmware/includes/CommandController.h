
#ifndef __COMMANDCONTROLLER_H
#define __COMMANDCONTROLLER_H

#include "Settings.h"

class CommandController {
    public:
        void setup(Settings settings);
        void loop(int test);

    private:
        Settings *settingsPtr;
        IMU *imuPtr;
};

#endif