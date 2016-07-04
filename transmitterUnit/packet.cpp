#include "packet.h"
#include <string.h>


#define RAIL_ANGLE_OFFSET 180
#define RAIL_ANGLE_MULTIPLIER 10
#define RAIL_ANGLE_MIN 0.0
#define RAIL_ANGLE_MAX 360.0

#define MAGNETIC_NORTH_ANGLE_OFFSET 0
#define MAGNETIC_NORTH_ANGLE_MULTIPLIER 10
#define MAGNETIC_NORTH_ANGLE_MIN 0.0
#define MAGNETIC_NORTH_ANGLE_MAX 360.0

#define WIND_SPEED_OFFSET 0
#define WIND_SPEED_MULTIPLIER 10
#define WIND_SPEED_MIN 0.0
#define WIND_SPEED_MAX 20.0

#define PRESSURE_OFFSET -900.0
#define PRESSURE_MULTIPLIER 10
#define PRESSURE_MIN 0.0
#define PRESSURE_MAX 200.0

#define TEMPERATURE_OFFSET 20.0
#define TEMPERATURE_MULTIPLIER 10
#define TEMPERATURE_MIN 0.0
#define TEMPERATURE_MAX 70.0

#define BATTERY_VOLTAGE_OFFSET -6
#define BATTERY_VOLTAGE_MULTIPLIER 100
#define BATTERY_VOLTAGE_MIN 0.0
#define BATTERY_VOLTAGE_MAX 2.4

#define GPS_FIX_AGE_MAX 255UL

#define ERROR_FLAG_OFFSET_BAROMETER 7
#define ERROR_FLAG_OFFSET_MAGNETOMETER 6
#define ERROR_FLAG_OFFSET_ACCELEROMETER 5
#define ERROR_FLAG_OFFSET_ANEMOMETER 4
#define ERROR_FLAG_OFFSET_WIND_VANE 3
#define ERROR_FLAG_OFFSET_Y 2
#define ERROR_FLAG_OFFSET_BATTERY 1
#define ERROR_FLAG_OFFSET_X 0

Packet::Packet()
{
	setLongitude(DEFAULT_LONGITUDE);
	setLatitude(DEFAULT_LATITUDE);
	setRailAngle(DEFAULT_RAIL_ANGLE);
	setMagneticNorthAngle(DEFAULT_MAGNETIC_NORTH_ANGLE);
	setWindSpeed(DEFAULT_WIND_SPEED);
	setWindDirection(DEFAULT_WIND_DIRECTION);
	setPressure(DEFAULT_PRESSURE);
	setTemperature(DEFAULT_TEMPERATURE);
	setBatteryVoltage(DEFAULT_BATTERY_VOLTAGE);
	setGPSFixAge(DEFAULT_GPS_FIX_AGE);
	setGPSFailedSentencesCount(DEFAULT_GPS_FAILED_SENTENCES_COUNT);
	setGPSStatus(DEFAULT_GPS_SATELLITES, DEFAULT_FIX_DETECTED);
	clearErrorFlags();
	isCRCUpToDate = false;
}

void Packet::setLongitude(int32_t lon)
{
	packet.longitude = lon;
	isCRCUpToDate = false;
}

void Packet::setLatitude(int32_t lat)
{
	packet.latitude = lat;
	isCRCUpToDate = false;
}

void Packet::setRailAngle(float angle)
{
	packet.railAngle = convertFloatToU16(angle, RAIL_ANGLE_OFFSET, RAIL_ANGLE_MULTIPLIER, RAIL_ANGLE_MIN, RAIL_ANGLE_MAX);
	isCRCUpToDate = false;
}

void Packet::setMagneticNorthAngle(float angle)
{
	packet.magneticNorthAngle = convertFloatToU16(angle, MAGNETIC_NORTH_ANGLE_OFFSET, MAGNETIC_NORTH_ANGLE_MULTIPLIER, MAGNETIC_NORTH_ANGLE_MIN, MAGNETIC_NORTH_ANGLE_MAX);
	isCRCUpToDate = false;
}

void Packet::setWindSpeed(float speed)
{
	packet.windSpeed = convertFloatToU8(speed, WIND_SPEED_OFFSET, WIND_SPEED_MULTIPLIER, WIND_SPEED_MIN, WIND_SPEED_MAX);
	isCRCUpToDate = false;
}

void Packet::setWindDirection(WindDirection dir)
{
	packet.windDirection = (uint8_t) dir;
	isCRCUpToDate = false;
}

void Packet::setPressure(float pres)
{
	packet.pressure = convertFloatToU16(pres, PRESSURE_OFFSET, PRESSURE_MULTIPLIER, PRESSURE_MIN, PRESSURE_MAX);
	isCRCUpToDate = false;
}

void Packet::setTemperature(float temp)
{
	packet.temperature = convertFloatToU16(temp, TEMPERATURE_OFFSET, TEMPERATURE_MULTIPLIER, TEMPERATURE_MIN, TEMPERATURE_MAX);
	isCRCUpToDate = false;
}

void Packet::setBatteryVoltage(float voltage)
{
	packet.batteryVoltage = convertFloatToU8(voltage, BATTERY_VOLTAGE_OFFSET, BATTERY_VOLTAGE_MULTIPLIER, BATTERY_VOLTAGE_MIN, BATTERY_VOLTAGE_MAX);
	isCRCUpToDate = false;
}

void Packet::setGPSFixAge(uint32_t age)
{
	if(age > GPS_FIX_AGE_MAX)
		age = GPS_FIX_AGE_MAX;
	packet.gpsFixAge = age;
	isCRCUpToDate = false;
}

void Packet::setGPSFailedSentencesCount(uint8_t failedCount)
{
	packet.gpsFailedSentencesCount = failedCount;
	isCRCUpToDate = false;
}

void Packet::setGPSStatus(uint8_t satellites, bool fixDetected)
{
	uint8_t status = 0;
	if(fixDetected) {
		status |= 0x80; //set oldest bit
	}
	status |= satellites;
	packet.gpsStatus = status;
	isCRCUpToDate = false;
}

void Packet::clearErrorFlags()
{
	packet.errors = 0;
	isCRCUpToDate = false;
}

void Packet::setBarometerErrorFlag(bool set)
{
	setErrorFlag(ERROR_FLAG_OFFSET_BAROMETER, set);
}

void Packet::setMagnetometerErrorFlag(bool set)
{
	setErrorFlag(ERROR_FLAG_OFFSET_MAGNETOMETER, set);
}

void Packet::setAccelerometerErrorFlag(bool set)
{
	setErrorFlag(ERROR_FLAG_OFFSET_ACCELEROMETER, set);
}

void Packet::setAnemometerErrorFlag(bool set)
{
	setErrorFlag(ERROR_FLAG_OFFSET_ANEMOMETER, set);
}

void Packet::setWindVaneErrorFlag(bool set)
{
	setErrorFlag(ERROR_FLAG_OFFSET_WIND_VANE, set);
}

void Packet::setBatteryErrorFlag(bool set)
{
	setErrorFlag(ERROR_FLAG_OFFSET_BATTERY, set);
}

void* Packet::getPacketData()
{
	updateCRC();
	return (void*) &packet;
}

uint8_t Packet::getPacketSize()
{
	return sizeof(packet);
}

#if INCL_RX_FUNC

void Packet::loadPacketData(void* packetData)
{
	memcpy((void*)&packet, packetData, sizeof(packet)); //copy data to local struct
}

uint8_t Packet::verifyCRC()
{
	uint8_t retCode = 0;
	if(packet.crc8 != calculateCRC8()) {
		retCode |= 0x01;
	}
	if(packet.crc16 != calculateCRC16()) {
		retCode |= 0x02;
	}
	if(packet.crc32 != calculateCRC32()) {
		retCode |= 0x04;
	}
	return retCode;
}

int32_t Packet::getLongitude()
{
	return packet.longitude;
}

int32_t Packet::getLatitude()
{
	return packet.latitude;
}

float Packet::getRailAngle()
{
	return rescaleFloat((float)packet.railAngle, RAIL_ANGLE_OFFSET, RAIL_ANGLE_MULTIPLIER);
}

float Packet::getMagneticNorthAngle()
{
	return rescaleFloat((float)packet.magneticNorthAngle, MAGNETIC_NORTH_ANGLE_OFFSET, MAGNETIC_NORTH_ANGLE_MULTIPLIER);
}

float Packet::getWindSpeed()
{
	return rescaleFloat((float)packet.windSpeed, WIND_SPEED_OFFSET, WIND_SPEED_MULTIPLIER);
}

WindDirection Packet::getWindDirection()
{
	WindDirection dir = (WindDirection)packet.windDirection;
	return dir;
}

float Packet::getPressure()
{
	return rescaleFloat((float)packet.pressure, PRESSURE_OFFSET, PRESSURE_MULTIPLIER);
}

float Packet::getTemperature()
{
	return rescaleFloat((float)packet.temperature, TEMPERATURE_OFFSET, TEMPERATURE_MULTIPLIER);
}

float Packet::getBatteryVoltage()
{
	return rescaleFloat((float)packet.batteryVoltage, BATTERY_VOLTAGE_OFFSET, BATTERY_VOLTAGE_MULTIPLIER);
}

uint8_t Packet::getErrors()
{
	return packet.errors;
}

uint8_t Packet::getGPSFixAge()
{
	return packet.gpsFixAge;
}

uint8_t Packet::getGPSFailedSentencesCount()
{
	return packet.gpsFailedSentencesCount;
}

uint8_t Packet::getGPSSatellitesCount()
{
	uint8_t satellites = packet.gpsStatus & 0x0F; //sat data stored in 4 LSB
	return satellites;
}

bool Packet::getGPSFixDetected()
{
	bool fixDetected = packet.gpsStatus & 0x80; //fix detected in MSB
	return fixDetected;
}

#endif

void Packet::updateCRC()
{
	packet.crc8 = calculateCRC8();
	packet.crc16 = calculateCRC16();
	packet.crc32 = calculateCRC32();
	isCRCUpToDate = true;
}

uint8_t Packet::calculateCRC8()
{
	uint8_t crc8 = packet.windSpeed + packet.windDirection + packet.batteryVoltage + packet.errors + packet.gpsFixAge + packet.gpsFailedSentencesCount + packet.gpsStatus;
	return crc8;
}

uint8_t Packet::calculateCRC16()
{
	uint16_t tmp16 = packet.railAngle + packet.magneticNorthAngle + packet.pressure + packet.temperature;
	uint8_t crc16 = tmp16 % 256;
	return crc16;
}

uint8_t Packet::calculateCRC32()
{
	int32_t tmp32 = packet.latitude + packet.longitude;
	if(tmp32 < 0) {
		tmp32 = -tmp32;
	}
	uint8_t crc32 = tmp32 % 256;
	return crc32;
}

float Packet::normalizeValue(float val, float min, float max)
{
	if(val < min)
		return min;
	if(val > max)
		return max;
	return val;
}

uint16_t Packet::convertFloatToU16(float val, float offset, float multiplier, float min, float max)
{
	return (uint16_t) scaleFloat(val, offset, multiplier, min, max);
}

uint8_t Packet::convertFloatToU8(float val, float offset, float multiplier, float min, float max)
{
	return (uint8_t) scaleFloat(val, offset, multiplier, min, max);
}

float Packet::scaleFloat(float val, float offset, float multiplier, float min, float max)
{
	val += offset;
	val = normalizeValue(val, min, max);
	val = val * multiplier + 0.5; // TOFIX: val can now be max+1 in some cases
	return val;
}

float Packet::rescaleFloat(float val, float offset, float multiplier)
{
	val = val / multiplier;
	val -= offset;
	return val;
}

void Packet::setErrorFlag(uint8_t index, bool set)
{
	uint8_t errors = packet.errors;
	if(index > 7) {
		return;
	}
	if(set) {
		errors |= (1<<index);
	} else {
		errors &=~(1<<index);
	}
	packet.errors = errors;
	isCRCUpToDate = false;
}