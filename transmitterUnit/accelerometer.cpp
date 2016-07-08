#include "accelerometer.h"

void Accelerometer::init()
{
	Wire.begin();
	mpu.initialize();
}

float Accelerometer::getAngle()
{
	//TODO if the device was disconnected after system's startup, then the device is uninitialized. Add code similiar to the one in Compass class
	int16_t ax, ay, az;
	mpu.getAcceleration(&ax, &ay, &az);
	float angle = 0;
	if(ay == 0) {
		angle = 90;
	} else {
		angle = -atan2(az, ay) * 180 / 3.1415;
	}
	return angle;
}

bool Accelerometer::isConnected()
{
	return mpu.testConnection();
}
