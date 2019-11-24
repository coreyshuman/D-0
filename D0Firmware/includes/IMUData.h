#ifndef __IMUDATA_H
#define __IMUDATA_H 

#include "helper_3dmath.h"

typedef struct _imu_data_t
{
    Quaternion q;           // [w, x, y, z]         quaternion container
    VectorInt16 accel;      // [x, y, z]            accel sensor measurements
    VectorInt16 gyro;       // [x, y, z]            gyro sensor measurements
    VectorFloat gravity;    // [x, y, z]            gravity vector
    VectorFloat ypr;        // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

}   imu_data_t;

#endif
