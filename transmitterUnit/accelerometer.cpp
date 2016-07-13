#include "accelerometer.h"

float Accelerometer::getValueImpl()
{
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

bool Accelerometer::initImpl()
{
	mpu.initialize();
	return isConnectedImpl();
}

bool Accelerometer::isConnectedImpl()
{
	return mpu.testConnection();
}
