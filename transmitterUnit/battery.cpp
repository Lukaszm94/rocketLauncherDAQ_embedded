#include "battery.h"
#include <Arduino.h>

float getBatteryVoltage()
{
	uint16_t adcReading = analogRead(BATTERY_ADC_CHANNEL);
	float batteryVoltage = adcReading * ADC_REF_V / 1024 * ADC_V_TO_REAL_V;
	return batteryVoltage;
}