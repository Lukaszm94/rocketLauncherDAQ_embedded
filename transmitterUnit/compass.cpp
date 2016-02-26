#include "compass.h"

Compass::Compass()
{
	connected = false;
}

void Compass::init()
{
	Wire.begin();
	mag.initialize();
	connected = mag.testConnection();
}

bool Compass::isConnected()
{
	return connected;
}

float Compass::getMagneticNorthAngle()
{
	if(!connected) {
		return 0.0;
	}
	int16_t mx, my, mz;
	mag.getHeading(&mx, &my, &mz);
	mx -= 71;
	my += 186;
	float heading = atan2(my, mx);
	if(heading < 0) {
		heading += 2 * M_PI;
	}
	return heading * 180/M_PI;
}