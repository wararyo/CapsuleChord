#include "GyroSensor.h"

void GyroSensor::begin() {
  IMU.MPU9250SelfTest(IMU.SelfTest);
  IMU.calibrateMPU9250(IMU.gyroBias, IMU.accelBias);
  IMU.initMPU9250();
}

void GyroSensor::update() {
    if(lastTime == 0) {
        lastTime = millis();
        return;
    }
    else {
        while (!IMU.readByte(MPU9250_ADDRESS, INT_STATUS) & 0x01);
        IMU.readGyroData(IMU.gyroCount); // Read the x/y/z adc values
        IMU.getGres();
        IMU.gx = (float)IMU.gyroCount[0] * IMU.gRes;
        IMU.gy = (float)IMU.gyroCount[1] * IMU.gRes;
        IMU.gz = (float)IMU.gyroCount[2] * IMU.gRes;

        latestX = IMU.gx;
        latestY = IMU.gy;
        latestZ = IMU.gz;

        deltaTime = (millis() - lastTime) / (float)1000;
        lastTime = millis();
    }
}

GyroSensor Gyro;