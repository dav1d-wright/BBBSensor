/*
 * BME280.h
 *
 *  Created on: 10 May 2016
 *      Author: David Wright
 */

#ifndef INC_BME280_H_
#define INC_BME280_H_
/*----------------------------------------------------------------------------*/
/* includes */
/*----------------------------------------------------------------------------*/
#include <I2CDevice.h>
/*----------------------------------------------------------------------------*/
/* precompiler definitions */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* type definitions */
/*----------------------------------------------------------------------------*/
typedef struct
{
	uint16_t m_uDigT1;/**<calibration T1 data*/
	int16_t m_iDigT2;/**<calibration T2 data*/
	int16_t m_iDigT3;/**<calibration T3 data*/
	uint16_t m_uDigP1;/**<calibration P1 data*/
	int16_t m_iDigP2;/**<calibration P2 data*/
	int16_t m_iDigP3;/**<calibration P3 data*/
	int16_t m_iDigP4;/**<calibration P4 data*/
	int16_t m_iDigP5;/**<calibration P5 data*/
	int16_t m_iDigP6;/**<calibration P6 data*/
	int16_t m_iDigP7;/**<calibration P7 data*/
	int16_t m_iDigP8;/**<calibration P8 data*/
	int16_t m_iDigP9;/**<calibration P9 data*/

	uint8_t  m_uDigH1;/**<calibration H1 data*/
	int16_t m_iDigH2;/**<calibration H2 data*/
	uint8_t  m_uDigH3;/**<calibration H3 data*/
	int16_t m_iDigH4;/**<calibration H4 data*/
	int16_t m_iDigH5;/**<calibration H5 data*/
	int8_t  m_iDigH6;/**<calibration H6 data*/

	int32_t m_iTFine;/**<calibration T_FINE data*/
}SBme280CalibParam ;
/*----------------------------------------------------------------------------*/
/* global variables */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* forward declarations */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* implementations */
/*----------------------------------------------------------------------------*/


class BME280: public I2CDevice
{
private:
	int32_t m_iTempUncomp; 			// Uncompensated temperature value
	int32_t m_iTempComp;			// Compensated temperature value in fixed point decimal
	double m_dTempDoub;				// Compensated temperature value in double
	uint32_t m_uPressUncomp;		// Uncompensated pressure value
	uint32_t m_uPressComp;			// Compensated pressure value
	double m_dPressDoub;			// Compensated pressure value in double point decimal
	int32_t m_iHumidUncomp; 		// Uncompensated humidity value
	uint32_t m_uHumidComp;			// Compensated humidity value in fixed point decimal
	double m_dHumidDoub;			// Compensated humidity value in double

	uint8_t m_uTempOversampling;
	uint8_t m_uPressOversampling;
	uint8_t m_uHumidOversampling;

	SBme280CalibParam m_sCalibParams;

	uint8_t m_uChipId;


public:
	BME280(uint8_t auBusNumber, uint16_t auDeviceId);
	~BME280(void);
	EError Init(void);
	EError GetAllCalibrationValues(void);
	EError ReadAllValuesUncomp(void);
	EError CompensateAllValues(void);
	EError ReadUncompTemperature(void);
	EError CompensateTemperature(void);
	EError ReadUncompPressure(void);
	EError CompensatePressure(void);
	EError ReadUncompHumidity(void);
	EError CompensateHumidity(void);
};


#endif /* INC_BME280_H_ */
