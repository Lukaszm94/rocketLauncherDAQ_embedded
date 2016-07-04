#ifndef COMPASS_H
#define COMPASS_H

#include <I2Cdev.h>
#include "HMC5883L_2.h"

class Compass
{
public:
	Compass();
	void init();
	bool isConnected();
	float getMagneticNorthAngle();
private:
	bool connected;
	void transformation(float uncalibrated_values[3]);
	void getHeading();
	HMC5883L compass;
	float calibrated_values[3];
	float xv, yv, zv;
};

#endif