#ifndef COMPASS_H
#define COMPASS_H

#include <I2Cdev.h>
#include <HMC5883L.h>

class Compass
{
public:
	Compass();
	void init();
	bool isConnected();
	float getMagneticNorthAngle();
private:
	bool connected;
	HMC5883L mag;
};

#endif