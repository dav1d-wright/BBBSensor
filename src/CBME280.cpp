/*
 * CBME280.cpp
 *
 *  Created on: 27.09.2016
 *      Author: z003nc3v
 */

#include <stdint.h>
#include "CBME280.h"
#include "BME280/bme280.h"
#include <stdio.h>
#include <iostream>
#include <unistd.h>
#include "Errors.h"

using namespace std;

/* Global CBME280 object */
CBME280 gcBme280;

int8_t writeCallBack(uint8_t auDeviceAddress, uint8_t auRegisterAddress, uint8_t* apuRegisterData, uint8_t auNumRegisters)
{
	int8_t iRet = 0;
	EError eRet = gcBme280.WriteRegisters(auRegisterAddress, auNumRegisters, apuRegisterData);
	if(eRet != eErrOk)
	{
		iRet = -1;
	}
	return iRet;
}

int8_t readCallBack(uint8_t auDeviceAddress, uint8_t auRegisterAddress, uint8_t* apuRegisterData, uint8_t auNumRegisters)
{
	int8_t iRet = 0;
	EError eRet = gcBme280.ReadRegisters(auRegisterAddress, auNumRegisters, apuRegisterData);
	if(eRet != eErrOk)
	{
		iRet = -1;
	}
	return iRet;
}

void milliSleep(const uint32_t auMillis)
{
	usleep(auMillis*1000);
}

CBME280::CBME280(uint8_t auPowerMode, uint8_t auStandbyDuration, uint8_t auOversampTemperature, uint8_t auOversampPressure, uint8_t auOversampHumidity, uint8_t auBusNumber, uint8_t auDeviceId):
		I2CDevice(auBusNumber, auDeviceId), m_uPowerMode(auPowerMode), m_uStandbyDuration(auStandbyDuration), m_uOversampTemperature(auOversampTemperature), m_uOversampPressure(auOversampPressure), m_uOversampHumidity(auOversampHumidity)
{

}

CBME280::~CBME280()
{

}

EError CBME280::init(void)
{

	int32_t iResult = 0;
	EError eError = this->Open();

	if(eError == eErrOk)
	{
		m_bme280.bus_read = readCallBack;
		m_bme280.bus_write = writeCallBack;
		m_bme280.dev_addr = BME280_I2C_ADDRESS2;
		m_bme280.delay_msec = 	milliSleep;

		iResult = bme280_init(&m_bme280);

		if(iResult != SUCCESS)
		{
			perror("BME280: Init failed.");
			eError = eErrBME280Init;
		}
		else
		{
			eError = this->setPowerMode(m_uPowerMode);
			if(eError == eErrOk)
			{
				eError = this->setStandbyDuration(m_uStandbyDuration);
			}
		}
	}
	return eError;
}

EError CBME280::setPowerMode(uint8_t auPowerMode)
{
	int32_t iResult = bme280_set_power_mode(auPowerMode);
	EError eError;
	if(iResult != SUCCESS)
	{
		perror("BME280: Setting power mode failed.");
		eError = eErrBME280SetPowerMode;
	}
	else
	{
		eError = eErrOk;
	}

	return eError;
}

EError CBME280::setStandbyDuration(uint8_t auStandbyDuration)
{
	int32_t iResult = bme280_set_standby_durn(auStandbyDuration);
	EError eError;
	if(iResult != SUCCESS)
	{
		perror("BME280: Setting standby duration failed.");
		eError = eErrBME280SetStandbyDuration;

	}
	else
	{
		eError = eErrOk;
	}

	return eError;
}

EError CBME280::setOversampHumidity(uint8_t auOversampHumid)
{
	int32_t iResult = bme280_set_oversamp_humidity(auOversampHumid);
	EError eError;
	if(iResult != SUCCESS)
	{
		perror("BME280: Setting humidity oversampling failed.");
		eError = eErrBME280SetOversampHumidity;
	}
	else
	{
		eError = eErrOk;
	}

	return eError;
}

EError CBME280::setOversampPressure(uint8_t auOversampPressure)
{
	int32_t iResult = bme280_set_oversamp_pressure(auOversampPressure);
	EError eError;
	if(iResult != SUCCESS)
	{
		perror("BME280: Setting pressure oversampling failed.");
		eError = eErrBME280SetOversampPressure;
	}
	else
	{
		eError = eErrOk;
	}

	return eError;
}

EError CBME280::setOversampTemperature(uint8_t auOversampTemp)
{
	int32_t iResult = bme280_set_oversamp_temperature(auOversampTemp);
	EError eError;
	if(iResult != SUCCESS)
	{
		perror("BME280: Setting temperature oversampling failed.");
		eError = eErrBME280SetOversampTemperature;
	}
	else
	{
		eError = eErrOk;
	}

	return eError;
}

EError CBME280::readUncompHumidity(void)
{
	int32_t iResult = bme280_read_uncomp_humidity(&m_iUncompHumidity);
	EError eError;
	if(iResult != SUCCESS)
	{
		perror("BME280: Reading uncompensated humidity failed.");
		eError = eErrBME280ReadUncompHumidity;
	}
	else
	{
		eError = eErrOk;
	}

	return eError;
}

EError CBME280::readUncompPressure(void)
{
	int32_t iResult = bme280_read_uncomp_pressure(&m_iUncompPressure);
	EError eError;
	if(iResult != SUCCESS)
	{
		perror("BME280: Reading uncompensated pressure failed.");
		eError = eErrBME280ReadUncompPressure;
	}
	else
	{
		eError = eErrOk;
	}

	return eError;
}

EError CBME280::readUncompTemperature(void)
{
	int32_t iResult = bme280_read_uncomp_temperature(&m_iUncompTemperature);
	EError eError;
	if(iResult != SUCCESS)
	{
		perror("BME280: Reading uncompensated temperature failed.");
		eError = eErrBME280ReadUncompTemperature;
	}
	else
	{
		eError = eErrOk;
	}

	return eError;
}

EError CBME280::readUncompValues(void)
{
	EError eError = this->readUncompTemperature();

	if(eError == eErrOk)
	{
		eError = this->readUncompPressure();

		if(eError == eErrOk)
		{
			eError = this->readUncompHumidity();
		}
	}

	return eError;
}

EError CBME280::compensateHumidity(void)
{
	EError eError = eErrOk;
	m_dCompHumidity = bme280_compensate_humidity_double(m_iUncompTemperature);
	m_uCompHumidity = bme280_compensate_humidity_int32(m_iUncompTemperature);

	return eError;
}

EError CBME280::compensatePressure(void)
{
	EError eError = eErrOk;
	m_dCompPressure = bme280_compensate_pressure_double(m_iUncompPressure);
	m_uCompPressure = bme280_compensate_pressure_int32(m_iUncompPressure);

	return eError;
}

EError CBME280::compensateTemperature(void)
{
	EError eError = eErrOk;
	m_dCompTemperature = bme280_compensate_temperature_double(m_iUncompTemperature);
	m_iCompTemperature = bme280_compensate_temperature_int32(m_iUncompTemperature);

	return eError;
}
