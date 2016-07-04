#include "anemometer.h"
#include <Arduino.h>

#define ANEMOMETER_CONSTANT 666.7
#define ANEMOMETER_MIN_DT 40 // in ms
#define ANEMOMETER_MAX_DT 2000 // in ms
#define ANEMOMETER_DEBUG_DDR DDRD
#define ANEMOMETER_DEBUG_PORT PORTD
#define ANEMOMETER_DEBUG_PIN PD5 // PD5
#define ANEMOMETER_CALCULATIONS_PIN 4

volatile unsigned long previousEdgeMs, currentEdgeMs;
volatile bool measurementConsumed;

void anemometerOnRisingEdge()
{
	//ANEMOMETER_DEBUG_PORT |= (1<<ANEMOMETER_DEBUG_PIN);
	previousEdgeMs = currentEdgeMs;
	currentEdgeMs = millis();
	measurementConsumed = false;
	//ANEMOMETER_DEBUG_PORT &=~(1<<ANEMOMETER_DEBUG_PIN);
}

Anemometer::Anemometer()
{
	ANEMOMETER_DEBUG_DDR |= (1<<ANEMOMETER_DEBUG_PIN);
	errorFlag = false;
	measurementConsumed = true;
	previousEdgeMs = 0;
	currentEdgeMs = 100;
	lastMeasuredWindSpeed = 0;
}

void Anemometer::update()
{
	unsigned long lastEdgeDt = millis() - currentEdgeMs;
	if(lastEdgeDt > ANEMOMETER_MAX_DT) {
		lastMeasuredWindSpeed = 0;
	}
	if(!measurementConsumed) {
		ANEMOMETER_DEBUG_PORT |= (1<<ANEMOMETER_DEBUG_PIN);
		consumeNewMeasurement();
		ANEMOMETER_DEBUG_PORT &=~(1<<ANEMOMETER_DEBUG_PIN);
		/*Serial.print("Wind speed: ");
		Serial.println(lastMeasuredWindSpeed);*/
	}
}

float Anemometer::getWindSpeed()
{
	return lastMeasuredWindSpeed;
}

bool Anemometer::getErrorFlag()
{
	return errorFlag;
}

void Anemometer::clearErrorFlag()
{
	errorFlag = false;
}

void Anemometer::consumeNewMeasurement()
{
	cli();
	unsigned long dt = currentEdgeMs - previousEdgeMs;
	measurementConsumed = true;
	sei();
	if(dt < ANEMOMETER_MIN_DT) {
		setErrorFlag();
		return;
	}
	float newWindSpeed;
	if(dt > ANEMOMETER_MAX_DT) {
		newWindSpeed = 0;
	} else {
		newWindSpeed = ANEMOMETER_CONSTANT / (float) dt;
	}
	//TODO filtering...
	lastMeasuredWindSpeed = newWindSpeed;
}

void Anemometer::setErrorFlag()
{
	errorFlag = true;
}