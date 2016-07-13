#include "accelerometer.h"

Accelerometer::Accelerometer()
{
	wasDisconnected = false;
}

void Accelerometer::init()
{
	Wire.begin();
	mpu.initialize();
	wasDisconnected = !isConnected();
}

float Accelerometer::getAngle()
{
	//TODO if the device was disconnected after system's startup, then the device is uninitialized. Add code similiar to the one in Compass class
	if(!isConnected()) {
		return 0.0;
	}
	if(wasDisconnected) {
		init();
	}
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
	bool connected = mpu.testConnection();
	if(!connected) {
		wasDisconnected = true;
	}
	return connected;
}
