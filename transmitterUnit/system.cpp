#include "system.h"
#include <Arduino.h>

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
	char address[6] = "skaTX";
	radio.begin();
	radio.setPALevel(RF24_PA_LOW);
	radio.openWritingPipe((uint8_t*)address);
	Serial.begin(115200);
}

void System::update()
{
	unsigned long txStartTime = micros();
	radio.write(packet.getPacketData(), packet.getPacketSize());
	Serial.print("Tx time: ");
	Serial.println(micros() - txStartTime);
	packet.setPressure(packet.getPressure() + 1);
}