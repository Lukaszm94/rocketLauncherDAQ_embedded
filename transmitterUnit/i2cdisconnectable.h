#ifndef I2CDISCONNECTABLE_H
#define I2CDISCONNECTABLE_H

class I2CDisconnectable
{
public:
	I2CDisconnectable();
	void init();
	bool isConnected();
	float getValue();

private:
	virtual float getValueImpl() = 0;
	virtual bool initImpl() = 0;
	virtual bool isConnectedImpl() = 0;

	bool wasDisconnected;
};

#endif // I2CDISCONNECTABLE_H
