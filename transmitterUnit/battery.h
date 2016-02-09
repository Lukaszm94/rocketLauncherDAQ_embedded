#ifndef BATTERY_H
#define BATTERY_H

#define ADC_REF_V 5.0
#define BATTERY_ADC_CHANNEL 1
#define ADC_V_TO_REAL_V ((10.0 + 10.0) / 10.0)

namespace Battery {
	float getBatteryVoltage();
}

#endif