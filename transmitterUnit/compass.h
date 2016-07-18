#ifndef COMPASS_H
#define COMPASS_H

#include <I2Cdev.h>
#include "HMC5883L.h"
#include "i2cdisconnectable.h"

struct Vector
{
	float XAxis;
	float YAxis;
	float ZAxis;
};

class Compass : public I2CDisconnectable
{
private:
	float getValueImpl();
	bool initImpl();
	bool isConnectedImpl();
	Vector transform(Vector rawValues);

	HMC5883L compass;
};

#endif
