#include <RF24.h>
#include "system.h"
#include "packet.h"
#include "windvane.h"

System sys;

void setup()
{
	sys.init();
}

void loop()
{
	delay(500);
	sys.update();
}