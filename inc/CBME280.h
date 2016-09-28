/*
 * CBME280.h
 *
 *  Created on: 27.09.2016
 *      Author: z003nc3v
 */

#ifndef CBME280_CBME280_H_
#define CBME280_CBME280_H_

#include "BME280/bme280.h"
#include "Errors.h"

class CBME280 {
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
	CBME280(uint8_t auPowerMode, uint8_t auStandbyDuration, uint8_t auOversampTemperature, uint8_t auOversampPressure, uint8_t auOversampHumidity);
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

#endif /* CBME280_CBME280_H_ */
