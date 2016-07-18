#include <RF24.h>
#include <Wire.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Adafruit_BMP085.h>
#include <TinyGPS.h>
#include <avr/wdt.h>
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
}

void loop()
{
	sys.update();
}

ISR(WDT_vect)
{

}
