#include <Arduino.h>
#include "../includes/IMU.h"
#include "../includes/Config.h"
#include "../includes/platform/Platform.h"
#include "MPU6050_6Axis_MotionApps20.h"

volatile bool IMU::mpuInterrupt = false;
MPU6050 mpu(MPU_I2C_ADDRESS);


void IMU::setup(imu_offsets_t offsets) {
    mpu.initialize();

    if(mpu.testConnection()) {
        // load and configure the DMP
        int devStatus = mpu.dmpInitialize();

        if (offsets.flags.valid) {
            setCalibrationOffsets(offsets);
            mpu.PrintActiveOffsets();
        }

        // make sure it worked (returns 0 if so)
        if (devStatus == 0) {
            mpu.setDMPEnabled(true);
            attachInterrupt(0, IMU::dmpDataReady, RISING);
            mpu.getIntStatus();
            dmpReady = true;

            // get expected DMP packet size for later comparison
            packetSize = mpu.dmpGetFIFOPacketSize();
            Serial.print(F("IMU Ready"));
        } else {
            // ERROR!
            // 1 = initial memory load failed
            // 2 = DMP configuration updates failed
            // (if it's going to break, usually the code will be 1)
            Serial.print(F("DMP Initialization failed (code "));
            Serial.print(devStatus);
            Serial.println(F(")"));
        }

    } else {
        Serial.print(F("ERR: IMU not connected."));
    }
}


void IMU::dmpDataReady() {
    IMU::mpuInterrupt = true;
}

void IMU::loop(int test) {
    // if programming failed, don't try to do anything
    if (!dmpReady) return;

    // received external pin interrupt from MPU
    if(IMU::mpuInterrupt) {
        // reset interrupt flag and get INT_STATUS byte
        IMU::mpuInterrupt = false;
        uint8_t mpuIntStatus = mpu.getIntStatus();

        // check for overflow (this should never happen unless our code is too inefficient)
        if ((mpuIntStatus & 0x10)) {
            // reset so we can continue cleanly
            mpu.resetFIFO();
            fifoCount = 0;
            Serial.println(F("FIFO overflow!"));

        // otherwise, check for DMP data ready interrupt (this should happen frequently)
        } else if (mpuIntStatus & 0x02) {
            // check for correct data length
            fifoCount = mpu.getFIFOCount();
        }
    }

    if (fifoCount >= packetSize) {
        uint8_t fifoBuffer[64]; // FIFO storage buffer
        float yprTemp[3];
        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        // display Euler angles in degrees
        mpu.dmpGetQuaternion(&imuData.q, fifoBuffer);
        mpu.dmpGetGravity(&imuData.gravity, &imuData.q);
        mpu.dmpGetYawPitchRoll(yprTemp, &imuData.q, &imuData.gravity);
        imuData.ypr.z = yprTemp[0];
        imuData.ypr.y = yprTemp[1];
        imuData.ypr.x = yprTemp[2];
    }
    
}

/********************************************************************
* Function: 	calibrate()
*
* Precondition: IMU::setup() has been called.
*
* Input: 		None
*
* Output:		Array of 6 int16 values representing offsets:
*               X Accel, Y Accel, Z Accel, X Gyro, Y Gyro, Z Gyro
*
* Side Effects:	None.
*
* Overview:     Calibrates accelerometers and gyros on the IMU.
*
*			
* Note:		 	Device should be level and stationary during calibration.
********************************************************************/
imu_offsets_t IMU::calibrate() {
    imu_offsets_t offsets;

    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.PrintActiveOffsets();
    offsets.xAccelOffset = mpu.getXAccelOffset();
    offsets.yAccelOffset = mpu.getYAccelOffset();
    offsets.zAccelOffset = mpu.getZAccelOffset();
    offsets.xGyroOffset = mpu.getXGyroOffset();
    offsets.yGyroOffset = mpu.getYGyroOffset();
    offsets.zGyroOffset = mpu.getZGyroOffset();
    offsets.flags.valid = 1;
    return offsets;
}

/********************************************************************
* Function: 	setCalibrationOffsets()
*
* Precondition: IMU::setup() has been called.
*
* Input: 		Array of 6 int16 values representing offsets:
*               X Accel, Y Accel, Z Accel, X Gyro, Y Gyro, Z Gyro
*
* Output:		None
*
* Side Effects:	None.
*
* Overview:     Write calibration offsets to IMU.
*
*			
* Note:		 	
********************************************************************/
void IMU::setCalibrationOffsets(imu_offsets_t offsets) {
    mpu.setXAccelOffset(offsets.xAccelOffset);
    mpu.setYAccelOffset(offsets.yAccelOffset);
    mpu.setZAccelOffset(offsets.zAccelOffset);
    mpu.setXGyroOffset(offsets.xGyroOffset);
    mpu.setYGyroOffset(offsets.yGyroOffset);
    mpu.setZGyroOffset(offsets.zGyroOffset);
}