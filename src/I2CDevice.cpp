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


#include <Errors.h>
#include <I2CDevice.h>
using namespace std;
/*----------------------------------------------------------------------------*/
/* definitions */
/*----------------------------------------------------------------------------*/
/* !Note!: use I2C2 with the following Pins on header P9 (closer to 5V plug):
 * GND = 1
 * Vin = 3
 * SCK = 19
 * SDI = 20
 */
const string sI2C0Path = "/dev/i2c-0";
const string sI2C1Path = "/dev/i2c-1";
const string sI2C2Path = "/dev/i2c-2";


/*----------------------------------------------------------------------------*/
/* implementations */
/*----------------------------------------------------------------------------*/
I2CDevice::I2CDevice(uint8_t auBusNumber, uint8_t auAddress):
m_uBusNumber(auBusNumber), m_uAddress(auAddress)
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
		sName = sI2C0Path;
	}
	else if(m_uBusNumber == 1)
	{
		sName = sI2C1Path;
	}
	else if(m_uBusNumber == 2)
	{
		sName = sI2C2Path;
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
			if(ioctl(m_iDeviceFile, I2C_SLAVE, m_uAddress) < 0)
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
	int8_t uValuesWritten = write(m_iDeviceFile, &auValue, 1);
	if(uValuesWritten != 1)
	{
		perror("I2C: Failed to write to device.\n");
		eError = eErrI2CCannotWrite;
	}
	return eError;
}

EError I2CDevice::WriteRegister(uint8_t auRegisterAddress, uint8_t auValue)
{
	return this->WriteRegisters(auRegisterAddress, 1, &auValue);
}

EError I2CDevice::WriteRegisters(uint8_t auRegisterAddress, uint8_t auNumRegisters, uint8_t* apuValues)
{
	EError eError = eErrOk;
	uint8_t* yuBuffer = new uint8_t[auNumRegisters + 1];
	yuBuffer[0] = auRegisterAddress;
	uint8_t uValuesWritten;

	for(uint8_t u = 0; u < auNumRegisters; u++)
	{
		yuBuffer[u+1] = apuValues[u];
	}

	uValuesWritten = write(m_iDeviceFile, yuBuffer, auNumRegisters + 1);
	if(uValuesWritten != auNumRegisters + 1)
	{
		perror("I2C: Failed to write to device.\n");
		eError = eErrI2CCannotWrite;
	}
	delete [] yuBuffer;
	return eError;
}

EError I2CDevice::ReadRegister(uint8_t auRegisterAddress, uint8_t* apuReadValue)
{
	return this->ReadRegisters(auRegisterAddress, 1, apuReadValue);
}

EError I2CDevice::ReadRegisters(uint8_t auRegisterAddress, uint8_t auNumRegisters, uint8_t* apuReadValues)
{
	EError eError;
	uint8_t uRegistersRead;

	eError = this->Write(auRegisterAddress);

	if(eError == eErrOk)
	{
		uRegistersRead = read(m_iDeviceFile, apuReadValues, auNumRegisters);
		if(uRegistersRead != auNumRegisters)
		{
			perror("I2C: Failed to read values from device register.\n");
			apuReadValues = NULL;
			eError = eErrI2CCannotRead;
		}
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
