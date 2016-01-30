#include <RF24.h>
#include "packet.h"
#define NRF_CE_PIN 9 // PB1
#define NRF_CSN_PIN 10 // PB2
#define DATA_BUFFER_SIZE 10

#define TEST_MODE 1

RF24 radio(NRF_CE_PIN, NRF_CSN_PIN);
Packet packet;
char dataBuffer[sizeof(PacketStruct)];

void printPacket(Packet pack)
{
	Serial.print("Lat: ");
	Serial.print(pack.getLatitude());
	Serial.print(", lon: ");
	Serial.println(pack.getLongitude());
	Serial.print("Rail angle: ");
	Serial.println(pack.getRailAngle());
	Serial.print("North angle: ");
	Serial.println(pack.getMagneticNorthAngle());
	Serial.print("Wind speed: ");
	Serial.println(pack.getWindSpeed());
	Serial.print("Wind direction: ");
	Serial.println((int)pack.getWindDirection());
	Serial.print("Pressure: ");
	Serial.println(pack.getPressure());
	Serial.print("Temperature: ");
	Serial.println(pack.getTemperature());
	Serial.print("Battery voltage: ");
	Serial.println(pack.getBatteryVoltage());
	Serial.print("Weather station errors: ");
	Serial.println(pack.getWeatherStationErrors());
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
	Serial.println(pack.calculateCRC32());
	
}


void setup()
{
	Serial.begin(115200);
	byte address[6] = "skaTX";
	radio.begin();
	radio.setPALevel(RF24_PA_LOW);
	radio.openReadingPipe(1, address);
	radio.startListening();
	Serial.println("Setup finished");
}

void loop()
{
	if(radio.available()) {
		delay(5); // wait for all the data to be received
		radio.read(dataBuffer, sizeof(dataBuffer));
		packet.loadPacketData(dataBuffer);
		printPacket(packet);
		if(packet.verifyCRC() == 0) {
			Serial.println("CRC ok");
		} else {
			Serial.print("CRC error: ");
			Serial.println(packet.verifyCRC());
		}
	}
	delay(100);
}