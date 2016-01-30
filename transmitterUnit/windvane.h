#ifndef WINDVANE_H
#define WINDVANE_H

#include "winddirection.h"

class WindVane
{
public:
	WindVane();
	void updateWindDirection(float windVaneVoltage, char* windDirectionBuffer); //converts voltage from wind vane to wind direction and stores it in text buffer as N, NE, SW etc., null terminated
private:

};

#endif