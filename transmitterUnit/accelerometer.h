#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <Arduino.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include "i2cdisconnectable.h"

class Accelerometer : public I2CDisconnectable
{
private:
	float getValueImpl();
	bool initImpl();
	bool isConnectedImpl();

	MPU6050 mpu;
};

#endif
