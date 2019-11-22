
#ifndef __IMU_H
#define __IMU_H

#include "MPU6050_6Axis_MotionApps20.h"
#include "SettingsData.h"

class IMU {
    public:
        void setup(imu_offsets_t offsets);
        void loop(int test);
        imu_offsets_t IMU::calibrate();
        void IMU::setCalibrationOffsets(imu_offsets_t offsets);

    private:
        MPU6050 mpu;
};

#endif