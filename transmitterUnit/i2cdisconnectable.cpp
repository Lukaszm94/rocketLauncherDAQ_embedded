#include "i2cdisconnectable.h"
#include <Wire.h>

I2CDisconnectable::I2CDisconnectable()
{
	wasDisconnected = false;
}

void I2CDisconnectable::init()
{
	Wire.begin();
	wasDisconnected = !initImpl();
}

bool I2CDisconnectable::isConnected()
{
	bool connected = isConnectedImpl();
	if(!connected) {
		wasDisconnected = true;
	}
	return connected;
}

float I2CDisconnectable::getValue()
{
	if(!isConnected()) {
		return 0.0;
	}
	if(wasDisconnected) {
		init(); // if the device was disconnected and now is connected (maybe because of poor connector contact with pins)
				//we need to reinitialize
	}
	return getValueImpl();
}
