#include <RF24.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Adafruit_BMP085.h>
#include "system.h"
#include "packet.h"
#include "windvane.h"
#include "anemometer.h"
#include "accelerometer.h"
#include "battery.h"

System sys;
Accelerometer acc;

void setup()
{
	sys.init();
	acc.init();
}

void loop()
{
	delay(500);
	Serial.println(acc.getAngle());
	sys.update();
}