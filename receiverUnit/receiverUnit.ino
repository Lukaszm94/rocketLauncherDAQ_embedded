#include <RF24.h>
#include "packet.h"

#define NRF_CE_PIN 9
#define NRF_CSN_PIN 10

#define DEBUG 0
#define LED_PIN 5

#define BINARY_PACKET_SIZE 26

RF24 radio(NRF_CE_PIN, NRF_CSN_PIN);
char dataBuffer[BINARY_PACKET_SIZE];

void setup()
{
	Serial.begin(115200);
	pinMode(LED_PIN, OUTPUT);
	byte address[6] = "skaTX";
	radio.begin();
	radio.setPALevel(RF24_PA_MAX);
	radio.openReadingPipe(1, address);
	if(radio.setDataRate(RF24_250KBPS)) {
		#if DEBUG
			Serial.println("Setting 250kbps successful");
		#endif
	} else {
		#if DEBUG
			Serial.println("Setting 250kbps failed");
		#endif
	}
	radio.startListening();
	#if DEBUG
		Serial.println("Setup finished");
		Serial.print("Payload size: ");
		Serial.println(radio.getPayloadSize());
	#endif
}

void loop()
{
	if(radio.available()) {
		digitalWrite(LED_PIN, HIGH);
		delay(5); // wait for all the data to be received
		radio.read(dataBuffer, BINARY_PACKET_SIZE);
		Serial.write(dataBuffer, BINARY_PACKET_SIZE);
	}
	delay(20);
	digitalWrite(LED_PIN, LOW);
}