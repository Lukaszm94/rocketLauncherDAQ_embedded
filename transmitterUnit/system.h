#ifndef SYSTEM_H
#define SYSTEM_H

#include <Arduino.h>
#include <RF24.h>
#include "packet.h"

class System
{
public:
	System();
	void init();
	void update();
private:
	Packet packet;
	RF24 radio;
};

#endif