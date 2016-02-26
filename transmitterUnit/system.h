#ifndef SYSTEM_H
#define SYSTEM_H

#include <Arduino.h>
#include <RF24.h>
#include <Adafruit_BMP085.h>
#include <TinyGPS.h>
#include "packet.h"
#include "anemometer.h"
#include "accelerometer.h"
#include "compass.h"


class System
{
public:
	System();
	void init();
	void update();
private:
	void initializeRadio();
	void updateSensors();
	void updatePacketData();
	void sendPacket();
	void updateGPS();
	void updateGPSPacketData();

	Anemometer anemometer;
	Accelerometer accelerometer;
	Packet packet;
	RF24 radio;
	Adafruit_BMP085 barometer;
	TinyGPS gps;
	Compass compass;
	unsigned long lastRadioUpdate;
};

#endif