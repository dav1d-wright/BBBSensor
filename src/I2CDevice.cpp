/*
 * I2CDevice.cpp
 *
 *  Created on: 10 May 2016
 *      Author: David Wright
 */
/*----------------------------------------------------------------------------*/
/* includes */
/*----------------------------------------------------------------------------*/
#include <stdint.h>
#include <I2CDevice.h>
/*----------------------------------------------------------------------------*/
/* definitions */
/*----------------------------------------------------------------------------*/
#define DF_BBB_I2C0_PATH "/dev/i2c-0"
#define DF_BBB_I2C1_PATH "/dev/i2c-1"
/*----------------------------------------------------------------------------*/
/* global variables */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* forward declarations */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* implementations */
/*----------------------------------------------------------------------------*/
I2CDevice::I2CDevice(uint8_t auBusNumber, uint16_t auDeviceId):
m_uBusNumber(auBusNumber), m_uDeviceId(auDeviceId)
{

}

I2CDevice::~I2CDevice(void)
{

}

uint8_t I2CDevice::Open(void)
{
	return 0;
}

void I2CDevice::Close(void)
{

}

uint8_t I2CDevice::Write(uint8_t auValue)
{
	return 0;
}

uint8_t I2CDevice::WriteRegister(uint8_t auRegisterAddress, uint8_t auValue)
{
	return 0;
}

uint8_t I2CDevice::ReadRegister(uint8_t auRegisterAddress)
{
	return 0;
}

uint8_t* I2CDevice::ReadRegisters(uint8_t auRegisterAddress, uint8_t auNumRegisters)
{
	return 0;
}

void I2CDevice::DumpRegisters(uint8_t auNumRegisters)
{

}
