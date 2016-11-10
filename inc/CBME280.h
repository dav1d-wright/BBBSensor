/*
 * CBME280.h
 *
 *  Created on: 27.09.2016
 *      Author: z003nc3v
 */

#ifndef CBME280_CBME280_H_
#define CBME280_CBME280_H_

#include "Errors.h"
#include "BME280/bme280.h"
#include "I2CDevice.h"

/*----------------------------------------------------------------------------*/
/* Function declarations */
/*----------------------------------------------------------------------------*/
int8_t writeCallBack(uint8_t auDeviceAddress, uint8_t auRegisterAddress, uint8_t* apuRegisterData, uint8_t auNumRegisters);
int8_t readCallBack(uint8_t auDeviceAddress, uint8_t auRegisterAddress, uint8_t* apuRegisterData, uint8_t auNumRegisters);
void milliSleep(const uint32_t auMillis);

class CBME280: public I2CDevice
{
private:
	bme280_t m_bme280;

	uint8_t m_uPowerMode;
	uint8_t m_uStandbyDuration;
	uint8_t m_uOversampTemperature;
	uint8_t m_uOversampPressure;
	uint8_t m_uOversampHumidity;

	int32_t m_iUncompTemperature = 0;
	int32_t m_iUncompPressure = 0;
	int32_t m_iUncompHumidity = 0;

	int32_t m_iCompTemperature = 0;
	uint32_t m_uCompPressure = 0;
	uint32_t m_uCompHumidity = 0;

	double m_dCompTemperature = 0.0;
	double m_dCompPressure = 0.0;
	double m_dCompHumidity = 0.0;


public:
	CBME280(uint8_t auPowerMode = BME280_NORMAL_MODE,
			uint8_t auStandbyDuration = BME280_STANDBY_TIME_1_MS,
			uint8_t auOversampTemperature = BME280_OVERSAMP_1X,
			uint8_t auOversampPressure = BME280_OVERSAMP_1X,
			uint8_t auOversampHumidity = BME280_OVERSAMP_1X,
			uint8_t auBusNumber = 2,
			uint8_t auAddress = 0x77);
	virtual ~CBME280();

	EError init(void);

	EError setPowerMode(uint8_t auPowerMode);
	EError setStandbyDuration(uint8_t auStandbyDuration);

	EError setOversampHumidity(uint8_t auOversampHumid);
	EError setOversampPressure(uint8_t auOversampPressure);
	EError setOversampTemperature(uint8_t auOversampTemp);

	EError readUncompHumidity(void);
	EError readUncompPressure(void);
	EError readUncompTemperature(void);
	EError readUncompValues(void);


	EError compensateHumidity(void);
	EError compensatePressure(void);
	EError compensateTemperature(void);
};

/* Global CBME280 object */
extern CBME280 gcBme280;

#endif /* CBME280_CBME280_H_ */
