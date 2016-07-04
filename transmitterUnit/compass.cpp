#include "compass.h"



//calibrated_values[3] is the global array where the calibrated data will be placed
//calibrated_values[3]: [0]=Xc, [1]=Yc, [2]=Zc
   
//transformation(float uncalibrated_values[3]) is the function of the magnetometer data correction 
//uncalibrated_values[3] is the array of the non calibrated magnetometer data
//uncalibrated_values[3]: [0]=Xnc, [1]=Ync, [2]=Znc
void Compass::transformation(float uncalibrated_values[3])
{
	//calibration_matrix[3][3] is the transformation matrix
	//replace M11, M12,..,M33 with your transformation matrix data
	double calibration_matrix[3][3] = 
	{
		{1.52, -0.001, -0.135},
		{-0.035, 1.454, -0.096},
		{0.081, 0.004, 1.633}  
	};
	//bias[3] is the bias
	//replace Bx, By, Bz with your bias data
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
	for (int i=0; i<3; ++i) calibrated_values[i] = result[i];
}

Compass::Compass()
{
	compass = HMC5883L();
	connected = true;
}

void Compass::init()
{
	Wire.begin();
	compass.SetScale(0.88);
	compass.SetMeasurementMode(Measurement_Continuous);
}

bool Compass::isConnected()
{
	return connected;
}

float Compass::getMagneticNorthAngle()
{
	if(!connected) {
		return 0.0;
	}
	float values_from_magnetometer[3];
	getHeading();
	/*Serial.print(xv);
	Serial.print(", ");
	Serial.print(yv);
	Serial.print(", ");
	Serial.print(zv);
	Serial.print(", mag:");
	Serial.println(sqrt(xv*xv + yv*yv + zv*zv));*/
	values_from_magnetometer[0] = xv;
	values_from_magnetometer[1] = yv;
	values_from_magnetometer[2] = zv;
	transformation(values_from_magnetometer);

	float heading = atan2(calibrated_values[1], calibrated_values[2]);
	if(heading < 0) {
		heading += 2 * M_PI;
	}
	return heading * 180/M_PI;
}

void Compass::getHeading()
{ 
	MagnetometerRaw raw = compass.ReadRawAxis();
	xv = (float)raw.XAxis;
	yv = (float)raw.YAxis;
	zv = (float)raw.ZAxis;
}