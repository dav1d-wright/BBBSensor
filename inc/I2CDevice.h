/*
 * I2CDevice.h
 *
 *  Created on: 10 May 2016
 *      Author: David Wright
 */

#ifndef INC_I2CDEVICE_H_
#define INC_I2CDEVICE_H_


class I2CDevice
{
private:
	uint8_t m_uBusNumber;
	uint8_t m_uDeviceId;

public:
	I2CDevice(uint8_t auBusNumber, uint16_t auDeviceId);
	~I2CDevice(void);
	virtual uint8_t Open(void);
	virtual void Close(void);
	virtual uint8_t Write(uint8_t auValue);
	virtual uint8_t WriteRegister(uint8_t auRegisterAddress, uint8_t auValue);
	virtual uint8_t ReadRegister(uint8_t auRegisterAddress);
	virtual uint8_t* ReadRegisters(uint8_t auRegisterAddress, uint8_t auNumRegisters);
	virtual void DumpRegisters(uint8_t auNumRegisters);
};


#endif /* INC_I2CDEVICE_H_ */
