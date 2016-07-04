#ifndef PACKET_H
#define PACKET_H
#include "winddirection.h"
#include <stdint.h>
//#define GPS_DATA_BUFFER_LENGTH 6
//#define WIND_DIRECTION_BUFFER_LENGTH 3

#define DEFAULT_LONGITUDE 0
#define DEFAULT_LATITUDE 0
#define DEFAULT_RAIL_ANGLE 45
#define DEFAULT_MAGNETIC_NORTH_ANGLE 30
#define DEFAULT_WIND_SPEED 5
#define DEFAULT_WIND_DIRECTION (Nx)
#define DEFAULT_PRESSURE 1013
#define DEFAULT_TEMPERATURE 22
#define DEFAULT_BATTERY_VOLTAGE 8
#define DEFAULT_GPS_FIX_AGE 255
#define DEFAULT_GPS_FAILED_SENTENCES_COUNT 0
#define DEFAULT_GPS_SATELLITES 0
#define DEFAULT_FIX_DETECTED false

#define INCL_RX_FUNC 1



struct PacketStruct
{
	uint16_t railAngle;
	uint16_t magneticNorthAngle;
	uint8_t windSpeed;
	uint8_t windDirection;
	uint16_t pressure;
	uint16_t temperature;
	int32_t latitude;
	int32_t longitude;
	uint8_t batteryVoltage;
	uint8_t errors;
	uint8_t gpsFixAge;
	uint8_t gpsFailedSentencesCount;
	uint8_t gpsStatus;
	uint8_t crc8;
	uint8_t crc16;
	uint8_t crc32;
};

class Packet
{
public:
	Packet();
	void setLongitude(int32_t lon);
	void setLatitude(int32_t lat);
	void setRailAngle(float angle); // in degrees
	void setMagneticNorthAngle(float angle); // in degrees
	void setWindSpeed(float speed); // in m/s
	void setWindDirection(WindDirection dir);
	void setPressure(float pres);
	void setTemperature(float temp);
	void setBatteryVoltage(float voltage);
	void setGPSFixAge(uint32_t age);
	void setGPSFailedSentencesCount(uint8_t failedCount);
	void setGPSStatus(uint8_t satellites, bool fixDetected);
	void clearErrorFlags();
	void setBarometerErrorFlag(bool set = true);
	void setMagnetometerErrorFlag(bool set = true);
	void setAccelerometerErrorFlag(bool set = true);
	void setAnemometerErrorFlag(bool set = true);
	void setWindVaneErrorFlag(bool set = true);
	void setBatteryErrorFlag(bool set = true);
	void* getPacketData();
	uint8_t getPacketSize();
#if INCL_RX_FUNC
	void loadPacketData(void* packetData);
	uint8_t verifyCRC(); // compares current CRC values with calculated and return status flag
	int32_t getLongitude();
	int32_t getLatitude();
	float getRailAngle();
	float getMagneticNorthAngle();
	float getWindSpeed();
	WindDirection getWindDirection();
	float getPressure();
	float getTemperature();
	float getBatteryVoltage();
	uint8_t getErrors();
	uint8_t getGPSFixAge();
	uint8_t getGPSFailedSentencesCount();
	uint8_t getGPSSatellitesCount();
	bool getGPSFixDetected();
#endif

#if TEST_MODE
private:
#endif

	void updateCRC();
	uint8_t calculateCRC8();
	uint8_t calculateCRC16();
	uint8_t calculateCRC32();
	float normalizeValue(float val, float min, float max);
	uint16_t convertFloatToU16(float val, float offset, float multiplier, float min, float max);
	uint8_t convertFloatToU8(float val, float offset, float multiplier, float min, float max);
	float scaleFloat(float val, float offset, float multiplier, float min, float max);
	float rescaleFloat(float val, float offset, float multiplier);
	void setErrorFlag(uint8_t index, bool set);

	PacketStruct packet;
	bool isCRCUpToDate;
};



#endif