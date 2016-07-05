#include "compass.h"

Compass::Compass()
{
	wasDisconnected = false;
}

void Compass::init()
{
	Wire.begin();
	wasDisconnected = !compass.begin();
}

bool Compass::isConnected()
{
	bool connected = compass.isConnected();
	if(!connected) {
		wasDisconnected = true;
	}
	return connected;
}

float Compass::getMagneticNorthAngle()
{
	if(!isConnected()) {
		return 0.0;
	}
	if(wasDisconnected) {
		Serial.println("REINITIALIZING");
		init(); // if the device was disconnected and now is connected (maybe because of poor connector contact with pins)
				//we need to reinitialize
	}
	Vector rawValues = compass.readRaw();
	/*Serial.print(xv);
	Serial.print(", ");
	Serial.print(yv);
	Serial.print(", ");
	Serial.print(zv);
	Serial.print(", mag:");
	Serial.println(sqrt(xv*xv + yv*yv + zv*zv));*/
	Vector calibratedValues = transform(rawValues);

	float heading = atan2(calibratedValues.YAxis, calibratedValues.ZAxis);
	if(heading < 0) {
		heading += 2 * M_PI;
	}
	return heading * 180/M_PI;
}

//function called to calibrate raw data from the device
Vector Compass::transform(Vector rawValues)
{
	float uncalibrated_values[3];
	uncalibrated_values[0] = rawValues.XAxis;
	uncalibrated_values[1] = rawValues.YAxis;
	uncalibrated_values[2] = rawValues.ZAxis;
	//calibration_matrix[3][3] is the transformation matrix
	double calibration_matrix[3][3] =
	{
		{1.52, -0.001, -0.135},
		{-0.035, 1.454, -0.096},
		{0.081, 0.004, 1.633}
	};

	double bias[3] =
	{
		522.2,
		248.3,
		188.2
	};
	//calculation
	for (int i=0; i<3; ++i) uncalibrated_values[i] = uncalibrated_values[i] - bias[i];
	float result[3] = {0, 0, 0};
	for (int i=0; i<3; ++i)
		for (int j=0; j<3; ++j)
			result[i] += calibration_matrix[i][j] * uncalibrated_values[j];

	Vector calibratedValues;
	calibratedValues.XAxis = result[0];
	calibratedValues.YAxis = result[1];
	calibratedValues.ZAxis = result[2];
	return calibratedValues;
}


