#ifndef WINDVANE_H
#define WINDVANE_H

#include "winddirection.h"
#include <stdint.h>

#define WIND_VANE_ADC_CHANNEL 0

class WindVane
{
public:
	WindVane();
	WindDirection getWindDirection();
private:
	uint8_t convertReadingToLutIndex(uint16_t reading);
	WindDirection convertLutIndexToWindDirection(uint8_t index);
};

#endif