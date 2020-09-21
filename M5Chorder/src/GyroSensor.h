#ifndef _GYROSENSOR_H_
#define _GYROSENSOR_H_

#include <string>
#include "utility/MPU9250.h"

class GyroSensor {
public:
    MPU9250 IMU;

    float latestX, latestY, latestZ;
    float deltaTime;

    void begin();

    void update();

protected:
    unsigned long lastTime;
};

extern GyroSensor Gyro;

#endif