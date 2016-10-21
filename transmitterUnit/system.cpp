#include "system.h"
#include <Arduino.h>
#include <avr/wdt.h>
#include "anemometer.h"
#include "battery.h"
#define NRF_CSN_PIN 10 // PB2
#define NRF_CE_PIN 8 // PB0

#define INFO_LED_PIN 6 // PD6
#define RADIO_UPDATE_RATE_MS 500

#define DEBUG 0

void printPacket(Packet pack)
{
	/*Serial.print("Lat: ");
	Serial.print(pack.getLatitude());
	Serial.print(", lon: ");
	Serial.println(pack.getLongitude());
	Serial.print("Rail angle: ");
	Serial.println(pack.getRailAngle());*/
	Serial.print("North angle: ");
	Serial.println(pack.getMagneticNorthAngle());
	/*Serial.print("Wind speed: ");
	Serial.println(pack.getWindSpeed());*/
	Serial.print("Wind direction: ");
	Serial.println((int)pack.getWindDirection());
	/*Serial.print("Pressure: ");
	Serial.println(pack.getPressure());
	Serial.print("Temperature: ");
	Serial.println(pack.getTemperature());
	Serial.print("Battery voltage: ");
	Serial.println(pack.getBatteryVoltage());
	Serial.print("Errors: ");
	Serial.println(pack.getErrors(), HEX);
	Serial.print("GPS fix age: ");
	Serial.println(pack.getGPSFixAge());
	Serial.print("GPS failed sentences: ");
	Serial.println(pack.getGPSFailedSentencesCount());
	Serial.print("Satellites: ");
	Serial.println(pack.getGPSSatellitesCount());
	Serial.print("GPS Fix Detected: ");
	Serial.println((int)pack.getGPSFixDetected());
	Serial.print("CRC8: ");
	Serial.println(pack.calculateCRC8());
	Serial.print("CRC16: ");
	Serial.println(pack.calculateCRC16());
	Serial.print("CRC32: ");
	Serial.println(pack.calculateCRC32());*/
	
}

System::System() : radio(NRF_CE_PIN, NRF_CSN_PIN)
{
	lastRadioUpdate = lastGPSStringTimestamp = 0;
}

void System::init()
{
	wdt_reset();
	pinMode(INFO_LED_PIN, OUTPUT);
	lastRadioUpdate = 0;
	Serial.begin(9600);
	delay(20);

	SPI.begin();
	initializeRadio();
	accelerometer.init();
	compass.init();
	if(!barometer.begin()) {
		packet.setBarometerErrorFlag();
	}
	setupWatchdog();
	attachInterrupt(digitalPinToInterrupt(2), anemometerOnRisingEdge, RISING);
}

void System::update()
{
	updateSensors();
	unsigned long dt = millis() - lastRadioUpdate;
	if(dt >= RADIO_UPDATE_RATE_MS) {
		digitalWrite(INFO_LED_PIN, HIGH);
		updatePacketData();
		lastRadioUpdate = millis();
		sendPacket();
		digitalWrite(INFO_LED_PIN, LOW);
		resetWatchdog();
		#if DEBUG
			Serial.print("Radio update, dt= ");
			Serial.println(dt);
		#endif
	}
}

void System::initializeRadio()
{
	char address[6] = "skaTX";
	if(!radio.begin()) {
		#if DEBUG
			Serial.println("Radio init failed!");
		#endif
		return;
	} else {
		#if DEBUG
			Serial.println("Radio init OK");
		#endif
	}
	delay(10);
	radio.setPALevel(RF24_PA_MAX);
	delay(10);
	radio.setDataRate(RF24_250KBPS);
	delay(10);
	radio.openWritingPipe((uint8_t*)address);
}

void System::setupWatchdog()
{
	cli();
	resetWatchdog();
	WDTCSR |= (1<<WDCE) | (1<<WDE); // enter WD configuration mode
	WDTCSR = (1<<WDIE) | (1<<WDE) | (1<<WDP3); // enable WD interrupt, enable system reset, 4000ms time-out
	sei();
}

void System::resetWatchdog()
{
	wdt_reset();
}

void System::updateSensors()
{
	anemometer.update();
	updateGPS();
}

void System::updatePacketData()
{
	updateBarometerData();
	updateMagnetometerData();
	updateAccelerometerData();
	updateAnemometerData();
	updateWindVaneData();
	updateGPSData();
	updateBatteryData();
}

void System::sendPacket()
{
	#if DEBUG
		Serial.println("Sending packet");
	#endif
	if(radio.write(packet.getPacketData(), packet.getPacketSize())) {
		#if DEBUG
			Serial.println("Transmission OK");
		#endif
	} else {
		#if DEBUG
			Serial.println("Transmission failed");
		#endif
	}
	//printPacket(packet);
}

void System::updateGPS()
{
	while(Serial.available()) {
		char c = Serial.read();
		if(gps.encode(c)) {
			lastGPSStringTimestamp = millis();
		}
	}
}

void System::updateBarometerData()
{
	float pressurePa = barometer.readPressure() / 100.0;
	float temperature = barometer.readTemperature();
	packet.setPressure(pressurePa);
	packet.setTemperature(temperature);
}

void System::updateMagnetometerData()
{
	if(!compass.isConnected()) {
		packet.setMagnetometerErrorFlag();
		//Serial.println("Mag not connected");
		return;
	}
	packet.setMagnetometerErrorFlag(false);
	float angle = compass.getValue();
	packet.setMagneticNorthAngle(angle);
}

void System::updateAccelerometerData()
{
	if(!accelerometer.isConnected()) {
		packet.setAccelerometerErrorFlag();
		//Serial.println("acc not connected");
		return;
	}
	packet.setAccelerometerErrorFlag(false);
	float angle = accelerometer.getValue();
	packet.setRailAngle(angle);
}

void System::updateAnemometerData()
{
	packet.setWindSpeed(anemometer.getWindSpeed());
}

void System::updateWindVaneData()
{
	WindDirection dir = windVane.getWindDirection(/*packet.getMagneticNorthAngle()*/);
	if(dir == UNKNOWN) {
		packet.setWindVaneErrorFlag();
		//Serial.println("wind vane not connected");
		return;
	}
	packet.setWindVaneErrorFlag(false);
	packet.setWindDirection(dir);
	#if DEBUG
		Serial.print("Wind direction(int): ");
		Serial.println((int) dir);
	#endif
}

void System::updateGPSData()
{
	/*unsigned long dt = millis() - lastGPSStringTimestamp;
	if(dt > 3000) {
		packet.setGPSErrorFlag();
		//Serial.println("gps not connected");
	} else {
		packet.setGPSErrorFlag(false);
	}*/
	long latitude, longitude;
	unsigned long fixAge;
	gps.get_position(&latitude, &longitude, &fixAge);
	packet.setLatitude(latitude);
	packet.setLongitude(longitude);
	unsigned long fixAgeS = fixAge / 1000;
	packet.setGPSFixAge(fixAgeS);
	if(fixAgeS > 255) {
		packet.setGPSStatus(0, false);
	} else {
		packet.setGPSStatus(gps.satellites(), true);
	}
}

void System::updateBatteryData()
{
	float voltage = Battery::getBatteryVoltage();
	if((voltage < 6.0) || (voltage > 8.5)) {
		//Serial.println("battery not connected");
		packet.setBatteryErrorFlag();
		return;
	}
	packet.setBatteryErrorFlag(false);
	packet.setBatteryVoltage(voltage);
}
