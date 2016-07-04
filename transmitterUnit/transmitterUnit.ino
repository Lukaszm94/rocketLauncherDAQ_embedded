#include <RF24.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Adafruit_BMP085.h>
#include <TinyGPS.h>
#include "system.h"
#include "packet.h"
#include "windvane.h"
#include "anemometer.h"
#include "accelerometer.h"
#include "battery.h"

System sys;


void setup()
{
	sys.init();
	DDRD |= (1<<PD5);
}

void loop()
{
	sys.update();
}