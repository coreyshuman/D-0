
#ifndef __IMU_H
#define __IMU_H


#include "SettingsData.h"
#include "IMUData.h"

class IMU {
    public:
        imu_data_t imuData;

        static volatile bool mpuInterrupt;     // indicates whether MPU interrupt pin has gone high
        static void dmpDataReady(void);

        void setup(imu_offsets_t settings);
        void loop(int test);
        imu_offsets_t calibrate();
        void setCalibrationOffsets(imu_offsets_t offsets);

    private:
        bool dmpReady = false;
        uint16_t packetSize = 0;
        uint16_t fifoCount = 0;
        
};

#endif