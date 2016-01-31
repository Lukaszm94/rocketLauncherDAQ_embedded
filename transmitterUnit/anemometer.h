#ifndef ANEMOMETER_H
#define ANEMOMETER_H

#define ANEMOMETER_PIN 2

void anemometerOnRisingEdge();

class Anemometer
{
public:
	Anemometer();
	void update();
	float getWindSpeed();
	bool getErrorFlag();
	void clearErrorFlag();

private:
	void consumeNewMeasurement();
	void setErrorFlag();

	float lastMeasuredWindSpeed;
	bool errorFlag; // set true if error occured
};

#endif