
#ifndef __IMU_H
#define __IMU_H


#include "SettingsData.h"

class IMU {
    public:
        void setup(imu_offsets_t settings);
        void loop(int test);
        imu_offsets_t calibrate();
        void setCalibrationOffsets(imu_offsets_t offsets);

    private:
        
};

#endif