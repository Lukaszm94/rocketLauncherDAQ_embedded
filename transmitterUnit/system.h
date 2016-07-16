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
#include "windvane.h"


class System
{
public:
	System();
	void init();
	void update();
private:
	void initializeRadio();
	void setupWatchdog();
	void resetWatchdog();
	void updateSensors();
	void updatePacketData();
	void sendPacket();
	void updateGPS();

	
	void updateBarometerData();
	void updateMagnetometerData();
	void updateAccelerometerData();
	void updateAnemometerData();
	void updateWindVaneData();
	void updateGPSData();
	void updateBatteryData();

	Anemometer anemometer;
	WindVane windVane;
	Accelerometer accelerometer;
	Packet packet;
	RF24 radio;
	Adafruit_BMP085 barometer;
	TinyGPS gps;
	Compass compass;
	unsigned long lastRadioUpdate;
	unsigned long lastGPSStringTimestamp;
};

#endif
