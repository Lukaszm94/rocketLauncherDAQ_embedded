#include "system.h"
#include <Arduino.h>
#include "anemometer.h"
#include "battery.h"
#define NRF_CSN_PIN 10 // PB2
#define NRF_CE_PIN 8 // PB0
#define DEBUG_PIN_1 PC3

#define INFO_LED_PIN 7 // PD7
#define RADIO_UPDATE_RATE_MS 1000

System::System() : radio(NRF_CE_PIN, NRF_CSN_PIN)
{
	
}

void System::init()
{
	DDRC |= (1<<DEBUG_PIN_1);
	pinMode(INFO_LED_PIN, OUTPUT);
	lastRadioUpdate = 0;
	initializeRadio();
	accelerometer.init();
	compass.init();
	if(!barometer.begin()) {
		//set some kind of error flag, blink LED, etc
	}
	Serial.begin(9600);
	//attachInterrupt(0, anemometerOnRisingEdge, RISING);
}

void System::update()
{
	updateSensors();
	unsigned long dt = millis() - lastRadioUpdate;
	if(dt >= RADIO_UPDATE_RATE_MS) {
		updatePacketData();
		lastRadioUpdate = millis();
		sendPacket();
	}
}

void System::initializeRadio()
{
	char address[6] = "skaTX";
	radio.begin();
	radio.setPALevel(RF24_PA_LOW);
	radio.openWritingPipe((uint8_t*)address);
}

void System::updateSensors()
{
	//anemometer.update();
	updateGPS();
}

void System::updatePacketData()
{
	updateGPSPacketData();
	packet.setRailAngle(accelerometer.getAngle());
	packet.setMagneticNorthAngle(compass.getMagneticNorthAngle());
	//packet.setWindSpeed(anemometer.getWindSpeed());
	//packet.setWindDirection(windVane.getWindDirection());
	packet.setPressure(barometer.readPressure() / 100.0);
	packet.setTemperature(barometer.readTemperature());
	packet.setBatteryVoltage(Battery::getBatteryVoltage());
}

void System::sendPacket()
{
	radio.write(packet.getPacketData(), packet.getPacketSize());
}

void System::updateGPS()
{
	while(Serial.available()) {
		char c = Serial.read();
		gps.encode(c);
	}
}

void System::updateGPSPacketData()
{
	long latitude, longitude;
	unsigned long fixAge;
	gps.get_position(&latitude, &longitude, &fixAge);
	packet.setLatitude(latitude);
	packet.setLongitude(longitude);
	unsigned long fixAgeS = fixAge / 1000;
	packet.setGPSFixAge(fixAgeS);
	if(fixAgeS > 255) {
		packet.setGPSStatus(0, false);
	} else {
		packet.setGPSStatus(gps.satellites(), true);
	}
	
}