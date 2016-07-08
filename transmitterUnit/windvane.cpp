#include "windvane.h"
#include "windvanelut.h"
#include <Arduino.h>

#define WIND_VANE_ADC_CHANNEL 0
#define EPS 4
#define UNKNOWN_DIRECTION 255

WindVane::WindVane()
{
}

WindDirection WindVane::getWindDirection(float magneticBearing)
{
	uint16_t reading = analogRead(WIND_VANE_ADC_CHANNEL);
	uint8_t index = convertReadingToLutIndex(reading);
	uint8_t indexCorrected = applyMagneticBearingCorrection(index, magneticBearing);
	return convertLutIndexToWindDirection(indexCorrected);
}

uint8_t WindVane::convertReadingToLutIndex(uint16_t reading)
{
	if(reading <= EPS) {
		return UNKNOWN_DIRECTION;
	}
	int lutSize = sizeof(windVaneLUT) / sizeof(uint16_t);
	for(int i = 0; i < lutSize; i++) {
		uint16_t lutValue = windVaneLUT[i];
		uint16_t lowerBound = lutValue - EPS;
		uint16_t upperBound = lutValue + EPS;
		if((reading >= lowerBound) && (reading <= upperBound)) {
			return i;
		}
	}
	return UNKNOWN_DIRECTION;
}

//magnetic bearing is an angle between 0 and 360 degrees
uint8_t WindVane::applyMagneticBearingCorrection(uint8_t indexWithoutCorrection, float magneticBearing)
{
	if(indexWithoutCorrection == UNKNOWN_DIRECTION) {
		return UNKNOWN_DIRECTION;
	}
	float resolution = 22.5;
	uint8_t indexShift = (magneticBearing + resolution / 2) / resolution;
	uint8_t indexWithCorrection = (indexWithoutCorrection + 16 - indexShift) % 16;
	return indexWithCorrection;
}

WindDirection WindVane::convertLutIndexToWindDirection(uint8_t index)
{
	if(index == UNKNOWN_DIRECTION) {
		return UNKNOWN;
	}
	index = index / 2; //let's stick to 45deg resolution
	WindDirection dir = (WindDirection) index;
	return dir;
}
