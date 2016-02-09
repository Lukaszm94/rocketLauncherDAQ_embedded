#include "system.h"
#include <Arduino.h>
#include "anemometer.h"
#include "battery.h"
#define NRF_CSN_PIN 10 // PB2
#define NRF_CE_PIN 8 // PB0
#define DEBUG_PIN_1 PC3

#define INFO_LED_PIN 7 // PD7

System::System() : radio(NRF_CE_PIN, NRF_CSN_PIN)
{
	
}

void System::init()
{
	DDRC |= (1<<DEBUG_PIN_1);
	pinMode(INFO_LED_PIN, OUTPUT);

	initializeRadio();
	accelerometer.init();
	if(!barometer.begin()) {
		//set some kind of error flag, blink LED, etc
	}
	Serial.begin(115200);
	attachInterrupt(0, anemometerOnRisingEdge, RISING);
}

void System::update()
{
	//unsigned long txStartTime = micros();
	
	//Serial.print("Tx time: ");
	//Serial.println(micros() - txStartTime);
	packet.setPressure(packet.getPressure() + 1);
	//anemometer.update();
}

void System::initializeRadio()
{
	char address[6] = "skaTX";
	radio.begin();
	radio.setPALevel(RF24_PA_MAX);
	radio.openWritingPipe((uint8_t*)address);
}

void System::updateSensors()
{
	anemometer.update();
	//gps.update(); //todo
}

void System::updatePacketData()
{
	//packet.setLongitude(gps.getLongitude());
	//packet.setLatitude(gps.getLatitude());
	packet.setRailAngle(accelerometer.getAngle());
	//packet.setMagneticNorthAngle(compass.getMagneticNorthAngle());
	packet.setWindSpeed(anemometer.getWindSpeed());
	//packet.setWindDirection(windVane.getWindDirection());
	packet.setPressure(barometer.readPressure() / 100.0);
	packet.setTemperature(barometer.readTemperature());
	packet.setBatteryVoltage(Battery::getBatteryVoltage());
}

void System::sendPacket()
{
	radio.write(packet.getPacketData(), packet.getPacketSize());
}