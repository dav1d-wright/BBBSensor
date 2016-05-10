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
#include <fstream>
#include <I2CDevice.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include <iostream>
#include <fcntl.h>
#include <sys/ioctl.h>
#include<unistd.h>
#include<linux/i2c.h>
#include<linux/i2c-dev.h>
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
	this->Open();
}

I2CDevice::~I2CDevice(void)
{
	if(m_iDeviceFile != -1)
	{
		this->Close();
	}
}

uint8_t I2CDevice::Open(void)
{
	uint8_t uRetVal = 0;
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
		uRetVal = 1;
	}

	if(uRetVal == 0)
	{
		m_iDeviceFile = open(sName.c_str(), O_RDWR);

		if(m_iDeviceFile < 0)
		{
			perror("I2C: Failed to open the bus.\n");
			uRetVal = 1;
		}

		if(uRetVal == 0)
		{
			if(ioctl(m_iDeviceFile, I2C_SLAVE, m_uDeviceId) < 0)
			{
				perror("I2C: Failed to connect to the device.\n");
				uRetVal = 1;
			}
		}
	}

	return uRetVal;
}

void I2CDevice::Close(void)
{
	close(m_iDeviceFile);
	m_iDeviceFile = -1;
}

uint8_t I2CDevice::Write(uint8_t auValue)
{
	uint8_t uRetVal = 0;

	if(write(m_iDeviceFile, &auValue, 1) != 1)
	{
		perror("I2C: Failed to write to device.\n");
		uRetVal = 1;
	}
	return uRetVal;
}

uint8_t I2CDevice::WriteRegister(uint8_t auRegisterAddress, uint8_t auValue)
{
	uint8_t uRetVal = 0;
	uint8_t yuBuffer[] = {auRegisterAddress, auValue};

	if(write(m_iDeviceFile, yuBuffer, 2) != 2)
	{
		perror("I2C: Failed to write to device.\n");
		uRetVal = 1;
	}
	return uRetVal;
}

uint8_t I2CDevice::ReadRegister(uint8_t auRegisterAddress, uint8_t* apuReadValue)
{
	return this->ReadRegisters(auRegisterAddress, 1, apuReadValue);
}

uint8_t I2CDevice::ReadRegisters(uint8_t auRegisterAddress, uint8_t auNumRegisters, uint8_t* apuReadValues)
{
	uint8_t uRetVal = 0;

	this->Write(auRegisterAddress);

	if(read(m_iDeviceFile, apuReadValues, auNumRegisters) != auNumRegisters)
	{
		perror("I2C: Failed to read values from device register.\n");
		apuReadValues = NULL;
		uRetVal = 1;
	}
	return 0;
}

uint8_t I2CDevice::DumpRegisters(uint8_t auNumRegisters)
{
	uint8_t uRetVal = 0;
	uint8_t* yuRegisters = new uint8_t[auNumRegisters];

	cout << "Dumping registers for debug purposes." << endl;

	if(this->ReadRegisters(0, auNumRegisters, yuRegisters) != 0)
	{
		perror("I2C: Reading registers for dump failed.\n");
		yuRegisters = NULL;
		uRetVal = 1;
	}
	else
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

	return uRetVal;
}
