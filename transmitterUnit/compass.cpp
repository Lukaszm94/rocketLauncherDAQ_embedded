#include "compass.h"

float Compass::getValueImpl()
{
	int samplesCount = 5;
	float sum = 0;
	for(int i = 0; i < samplesCount; i++) {
		Vector rawValues;
		rawValues.XAxis = compass.getHeadingX();
		rawValues.YAxis = compass.getHeadingY();
		rawValues.ZAxis = compass.getHeadingZ();
		Vector calibratedValues = transform(rawValues);
		float heading = atan2(calibratedValues.ZAxis, calibratedValues.YAxis);
		if(heading < 0) {
			heading += 2 * M_PI;
		}
		sum += heading;
		//TODO when angle is close to 0deg, values like 0.2 and 359.8 get summed and averaged, which results in erroneous output
	}
	float heading = sum / samplesCount;
	return heading * 180/M_PI;
}

bool Compass::initImpl()
{
	compass.initialize();
	compass.setGain(HMC5883L_GAIN_1370);
	return compass.testConnection();
}

bool Compass::isConnectedImpl()
{
	return compass.testConnection();
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


