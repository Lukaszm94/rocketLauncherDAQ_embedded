#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include <Arduino.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>

class Accelerometer
{
public:
	void init();
	float getAngle();
	bool isConnected();
private:
	MPU6050 mpu;
};

#endif