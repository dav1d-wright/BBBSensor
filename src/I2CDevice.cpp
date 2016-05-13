/**----------------------------------------------------------------------------
	\file		I2CDevice.cpp
	\author		D. Wright
	\version	0

	\brief Class that implements a generic I2C Device on an embedded linux platform.

	Long comment on this file.

  ----------------------------------------------------------------------------*/

/*!	\defgroup Devices Higher functionality devices.
	\{
*/

/*!	\defgroup DEV_I2C I2C Device (I2C)
 	 * \brief The I2C device handles the I2C communication from the BeagleBone master to the slave device.
	\{
*/

/*----------------------------------------------------------------------------*/
/* includes */
/*----------------------------------------------------------------------------*/
#include <stdint.h>
#include <fstream>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <iostream>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>

#include <Errors.h>
#include <I2CDevice.h>
using namespace std;
/*----------------------------------------------------------------------------*/
/* definitions */
/*----------------------------------------------------------------------------*/
#define DF_BBB_PATH_I2C0 "/dev/i2c-0"
#define DF_BBB_PATH_I2C1 "/dev/i2c-1"
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
	m_iDeviceFile = -1;
}

I2CDevice::~I2CDevice(void)
{
	if(m_iDeviceFile != -1)
	{
		this->Close();
	}
}

/*----------------------------------------------------------------------------*/
/*!
	\pre
	Constructor has been called.

	\post
	BeagleBone has been connected to I2C slave.

	Opens an I2C bus with the assigned bus number and then connects to this I2C bus.
*/
/*----------------------------------------------------------------------------*/
EError I2CDevice::Open(void)
{
	EError eError = eErrOk;
	string sName;
	if(m_uBusNumber == 0)
	{
		sName = DF_BBB_PATH_I2C0;
	}
	else if(m_uBusNumber == 1)
	{
		sName = DF_BBB_PATH_I2C1;
	}
	else
	{
		perror("I2C: Incorrect bus number.\n");
		eError = eErrI2CBusNumber;
	}

	if(eError == eErrOk)
	{
		m_iDeviceFile = open(sName.c_str(), O_RDWR);

		if(m_iDeviceFile < 0)
		{
			perror("I2C: Failed to open the bus.\n");
			eError = eErrI2CCannotOpenBus;
		}

		if(eError == eErrOk)
		{
			if(ioctl(m_iDeviceFile, I2C_SLAVE, m_uDeviceId) < 0)
			{
				perror("I2C: Failed to connect to the device.\n");
				eError = eErrI2CCannotConnect;
			}
		}
	}

	return eError;
}

/*----------------------------------------------------------------------------*/
/*!
	\pre
	None.

	\post
	BeagleBone has been disconnected from I2C slave.

	Closes the connection to the I2C bus.
*/
/*----------------------------------------------------------------------------*/
void I2CDevice::Close(void)
{
	close(m_iDeviceFile);
	m_iDeviceFile = -1;
}

EError I2CDevice::Write(uint8_t auValue)
{
	EError eError = eErrOk;

	if(write(m_iDeviceFile, &auValue, 1) != 1)
	{
		perror("I2C: Failed to write to device.\n");
		eError = eErrI2CCannotWrite;
	}
	return eError;
}

EError I2CDevice::WriteRegister(uint8_t auRegisterAddress, uint8_t auValue)
{
	EError eError = eErrOk;
	uint8_t yuBuffer[] = {auRegisterAddress, auValue};

	if(write(m_iDeviceFile, yuBuffer, 2) != 2)
	{
		perror("I2C: Failed to write to device.\n");
		eError = eErrI2CCannotWrite;
	}
	return eError;
}

EError I2CDevice::ReadRegister(uint8_t auRegisterAddress, uint8_t* apuReadValue)
{
	return this->ReadRegisters(auRegisterAddress, 1, apuReadValue);
}

EError I2CDevice::ReadRegisters(uint8_t auRegisterAddress, uint8_t auNumRegisters, uint8_t* apuReadValues)
{
	EError eError = eErrOk;

	this->Write(auRegisterAddress);

	if(read(m_iDeviceFile, apuReadValues, auNumRegisters) != auNumRegisters)
	{
		perror("I2C: Failed to read values from device register.\n");
		apuReadValues = NULL;
		eError = eErrI2CCannotRead;
	}

	return eError;
}

EError I2CDevice::DumpRegisters(uint8_t auNumRegisters)
{
	EError eError = eErrOk;
	uint8_t* yuRegisters = new uint8_t[auNumRegisters];

	cout << "Dumping registers for debug purposes." << endl;

	eError = this->ReadRegisters(0, auNumRegisters, yuRegisters);

	if(eError != eErrOk)
	{
		for(uint8_t uLoop = 0; uLoop < auNumRegisters; uLoop++)
		{
			cout << "0x" << hex << yuRegisters[uLoop] << " ";
			if((uLoop % 16) == 15)
			{
				cout << endl;
			}
		}
	}

	delete[] yuRegisters;

	return eError;
}
