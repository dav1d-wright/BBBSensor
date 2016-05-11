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
	int32_t m_iDeviceFile;

public:
	I2CDevice(uint8_t auBusNumber, uint16_t auDeviceId);
	~I2CDevice(void);
	virtual EError Open(void);
	virtual void Close(void);
	virtual EError Write(uint8_t auValue);
	virtual EError WriteRegister(uint8_t auRegisterAddress, uint8_t auValue);
	virtual EError ReadRegister(uint8_t auRegisterAddress, uint8_t* apuReadValue);
	virtual EError ReadRegisters(uint8_t auRegisterAddress, uint8_t auNumRegisters, uint8_t* apuReadValues);
	virtual EError DumpRegisters(uint8_t auNumRegisters);
};


#endif /* INC_I2CDEVICE_H_ */
