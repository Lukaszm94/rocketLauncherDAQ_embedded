#include <RF24.h>
#include "system.h"
#include "packet.h"
#include "windvane.h"
#include "anemometer.h"

System sys;


void setup()
{
	sys.init();
	attachInterrupt(0, anemometerOnRisingEdge, RISING);
}

void loop()
{
	//delay(50);
	sys.update();
}