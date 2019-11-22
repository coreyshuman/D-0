#include <Arduino.h>
#include "../includes/IMU.h"
#include "../includes/Config.h"
#include "../includes/platform/Platform.h"


void IMU::setup(imu_offsets_t offsets) {
    mpu = MPU6050(MPU_I2C_ADDRESS);
    mpu.initialize();
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
    // load and configure the DMP
    Serial.println(F("Initializing DMP..."));
    int devStatus = mpu.dmpInitialize();

    if (offsets != nullptr) {
        setCalibrationOffsets(offsets);
        mpu.PrintActiveOffsets();
    }

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        Serial.println(F("Enabling DMP..."));
        mpu.setDMPEnabled(true);

        // enable Arduino interrupt detection
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        attachInterrupt(0, dmpDataReady, RISING);
        mpuIntStatus = mpu.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }

}

void IMU::loop(int test) {

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
int16_t* IMU::setCalibrationOffsets(imu_offsets_t offsets) {
    mpu.setXAccelOffset(offsets.xAccelOffset);
    mpu.setYAccelOffset(offsets.yAccelOffset);
    mpu.setZAccelOffset(offsets.zAccelOffset);
    mpu.setXGyroOffset(offsets.xGyroOffset);
    mpu.setYGyroOffset(offsets.yGyroOffset);
    mpu.setZGyroOffset(offsets.zGyroOffset);
}