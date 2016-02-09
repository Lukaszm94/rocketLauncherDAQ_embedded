#ifndef WINDVANE_H
#define WINDVANE_H

#include "winddirection.h"

#define WIND_VANE_ADC_CHANNEL 0

class WindVane
{
public:
	WindVane();
	WindDirection getWindDirection();
private:
	
};

#endif