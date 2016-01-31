#ifndef SYSTEM_H
#define SYSTEM_H

#include <Arduino.h>
#include <RF24.h>
#include "packet.h"
#include "anemometer.h"


class System
{
public:
	System();
	void init();
	void update();
private:
	Anemometer anemometer;
	Packet packet;
	RF24 radio;
};

#endif