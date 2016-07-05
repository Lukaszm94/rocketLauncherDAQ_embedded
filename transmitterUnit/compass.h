#ifndef COMPASS_H
#define COMPASS_H

#include <I2Cdev.h>
#include "HMC5883L.h"

class Compass
{
public:
	Compass();
	void init();
	bool isConnected();
	float getMagneticNorthAngle();
private:
	Vector transform(Vector rawValues);

	bool wasDisconnected;
	HMC5883L compass;
};

#endif
