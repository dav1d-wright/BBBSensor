/*
 * BME280.cpp
 *
 *  Created on: 10 May 2016
 *      Author: David Wright
 */
/*----------------------------------------------------------------------------*/
/* includes */
/*----------------------------------------------------------------------------*/
#include <stdint.h>
#include <fstream>
#include <Errors.h>
#include <BME280.h>

/*----------------------------------------------------------------------------*/
/* definitions */
/*----------------------------------------------------------------------------*/
// Numeric defines
#define	DF_BME280_PRESS_TEMP_CALIB_DATA_LENGTH	        26
#define	DF_BME280_HUMIDITY_CALIB_DATA_LENGTH	        7
#define	DF_BME280_GEN_READ_WRITE_DATA_LENGTH		    1
#define	DF_BME280_HUMIDITY_DATA_LENGTH				    2
#define	DF_BME280_TEMPERATURE_DATA_LENGTH			    3
#define	DF_BME280_PRESSURE_DATA_LENGTH				    3
#define	DF_BME280_ALL_DATA_FRAME_LENGTH				    8
#define	DF_BME280_INIT_VALUE						    0
#define	DF_BME280_INVALID_DATA						    0

// Address defines
#define DF_BME280_I2C_ADDRESS1                  		0x76U
#define DF_BME280_I2C_ADDRESS2                  		0x77U

// Power mode defines
#define DF_BME280_SLEEP_MODE                    		0x00U
#define DF_BME280_FORCED_MODE                   		0x01U
#define DF_BME280_NORMAL_MODE                   		0x03U
#define DF_BME280_SOFT_RESET_CODE               		0xB6U

// standby defines
#define DF_BME280_STANDBY_TIME_1_MS             		0x00U
#define DF_BME280_STANDBY_TIME_63_MS            		0x01U
#define DF_BME280_STANDBY_TIME_125_MS			 		0x02U
#define DF_BME280_STANDBY_TIME_250_MS            		0x03U
#define DF_BME280_STANDBY_TIME_500_MS            		0x04U
#define DF_BME280_STANDBY_TIME_1000_MS           		0x05U
#define DF_BME280_STANDBY_TIME_10_MS             		0x06U
#define DF_BME280_STANDBY_TIME_20_MS             		0x07U

// oversampling defines
#define DF_BME280_OVERSAMP_SKIPPED          			0x00U
#define DF_BME280_OVERSAMP_1X               			0x01U
#define DF_BME280_OVERSAMP_2X               			0x02U
#define DF_BME280_OVERSAMP_4X               			0x03U
#define DF_BME280_OVERSAMP_8X               			0x04U
#define DF_BME280_OVERSAMP_16X              			0x05U

// work mode defines
#define DF_BME280_ULTRALOWPOWER_MODE          			0x00U
#define DF_BME280_LOWPOWER_MODE               			0x01U
#define DF_BME280_STANDARDRESOLUTION_MODE     			0x02U
#define DF_BME280_HIGHRESOLUTION_MODE         			0x03U
#define DF_BME280_ULTRAHIGHRESOLUTION_MODE    			0x04U

#define DF_BME280_ULTRALOWPOWER_OSRS_P          		DF_BME280_OVERSAMP_1X
#define DF_BME280_ULTRALOWPOWER_OSRS_T          		DF_BME280_OVERSAMP_1X

#define DF_BME280_LOWPOWER_OSRS_P               		DF_BME280_OVERSAMP_2X
#define DF_BME280_LOWPOWER_OSRS_T               		DF_BME280_OVERSAMP_1X

#define DF_BME280_STANDARDRESOLUTION_OSRS_P     		DF_BME280_OVERSAMP_4X
#define DF_BME280_STANDARDRESOLUTION_OSRS_T     		DF_BME280_OVERSAMP_1X

#define DF_BME280_HIGHRESOLUTION_OSRS_P         		DF_BME280_OVERSAMP_8X
#define DF_BME280_HIGHRESOLUTION_OSRS_T         		DF_BME280_OVERSAMP_1X

#define DF_BME280_ULTRAHIGHRESOLUTION_OSRS_P    		DF_BME280_OVERSAMP_16X
#define DF_BME280_ULTRAHIGHRESOLUTION_OSRS_T    		DF_BME280_OVERSAMP_2X

#define DF_BME280_STANDARD_OVERSAMP_HUMIDITY			DF_BME280_OVERSAMP_1X

// filter defines
#define DF_BME280_FILTER_COEFF_OFF               		0x00U
#define DF_BME280_FILTER_COEFF_2                 		0x01U
#define DF_BME280_FILTER_COEFF_4                 		0x02U
#define DF_BME280_FILTER_COEFF_8                 		0x03U
#define DF_BME280_FILTER_COEFF_16                		0x04U

// delay defines
#define DF_BME280_T_INIT_MAX                            20		/* 20/16 = 1.25 ms */
#define DF_BME280_T_MEASURE_PER_OSRS_MAX                37		/* 37/16 = 2.3125 ms*/
#define DF_BME280_T_SETUP_PRESSURE_MAX                  10		/* 10/16 = 0.625 ms */
#define DF_BME280_T_SETUP_HUMIDITY_MAX                  10		/* 10/16 = 0.625 ms */

// defines for array size of data
#define	DF_BME280_HUMIDITY_DATA_SIZE					2
#define	DF_BME280_TEMPERATURE_DATA_SIZE					3
#define	DF_BME280_PRESSURE_DATA_SIZE					3
#define	DF_BME280_DATA_FRAME_SIZE						8
/**< data frames includes temperature,
pressure and humidity*/
#define	DF_BME280_CALIB_DATA_SIZE						26

#define	DF_BME280_TEMPERATURE_MSB_DATA					0
#define	DF_BME280_TEMPERATURE_LSB_DATA					1
#define	DF_BME280_TEMPERATURE_XLSB_DATA					2
#define	DF_BME280_PRESSURE_MSB_DATA						0
#define	DF_BME280_PRESSURE_LSB_DATA						1
#define	DF_BME280_PRESSURE_XLSB_DATA	    			2
#define	DF_BME280_HUMIDITY_MSB_DATA						0
#define	DF_BME280_HUMIDITY_LSB_DATA						1

#define	DF_BME280_DATA_FRAME_PRESSURE_MSB_BYTE	    	0
#define	DF_BME280_DATA_FRAME_PRESSURE_LSB_BYTE			1
#define	DF_BME280_DATA_FRAME_PRESSURE_XLSB_BYTE			2
#define	DF_BME280_DATA_FRAME_TEMPERATURE_MSB_BYTE		3
#define	DF_BME280_DATA_FRAME_TEMPERATURE_LSB_BYTE		4
#define	DF_BME280_DATA_FRAME_TEMPERATURE_XLSB_BYTE		5
#define	DF_BME280_DATA_FRAME_HUMIDITY_MSB_BYTE			6
#define	DF_BME280_DATA_FRAME_HUMIDITY_LSB_BYTE			7

// defines for array parameters for calibration
#define	DF_BME280_TEMPERATURE_CALIB_DIG_T1_LSB			0
#define	DF_BME280_TEMPERATURE_CALIB_DIG_T1_MSB			1
#define	DF_BME280_TEMPERATURE_CALIB_DIG_T2_LSB			2
#define	DF_BME280_TEMPERATURE_CALIB_DIG_T2_MSB			3
#define	DF_BME280_TEMPERATURE_CALIB_DIG_T3_LSB			4
#define	DF_BME280_TEMPERATURE_CALIB_DIG_T3_MSB			5
#define	DF_BME280_PRESSURE_CALIB_DIG_P1_LSB       		6
#define	DF_BME280_PRESSURE_CALIB_DIG_P1_MSB       		7
#define	DF_BME280_PRESSURE_CALIB_DIG_P2_LSB       		8
#define	DF_BME280_PRESSURE_CALIB_DIG_P2_MSB       		9
#define	DF_BME280_PRESSURE_CALIB_DIG_P3_LSB       		10
#define	DF_BME280_PRESSURE_CALIB_DIG_P3_MSB       		11
#define	DF_BME280_PRESSURE_CALIB_DIG_P4_LSB       		12
#define	DF_BME280_PRESSURE_CALIB_DIG_P4_MSB       		13
#define	DF_BME280_PRESSURE_CALIB_DIG_P5_LSB       		14
#define	DF_BME280_PRESSURE_CALIB_DIG_P5_MSB       		15
#define	DF_BME280_PRESSURE_CALIB_DIG_P6_LSB       		16
#define	DF_BME280_PRESSURE_CALIB_DIG_P6_MSB       		17
#define	DF_BME280_PRESSURE_CALIB_DIG_P7_LSB       		18
#define	DF_BME280_PRESSURE_CALIB_DIG_P7_MSB       		19
#define	DF_BME280_PRESSURE_CALIB_DIG_P8_LSB       		20
#define	DF_BME280_PRESSURE_CALIB_DIG_P8_MSB       		21
#define	DF_BME280_PRESSURE_CALIB_DIG_P9_LSB       		22
#define	DF_BME280_PRESSURE_CALIB_DIG_P9_MSB       		23
#define	DF_BME280_HUMIDITY_CALIB_DIG_H1           		25
#define	DF_BME280_HUMIDITY_CALIB_DIG_H2_LSB				0
#define	DF_BME280_HUMIDITY_CALIB_DIG_H2_MSB				1
#define	DF_BME280_HUMIDITY_CALIB_DIG_H3					2
#define	DF_BME280_HUMIDITY_CALIB_DIG_H4_MSB				3
#define	DF_BME280_HUMIDITY_CALIB_DIG_H4_LSB				4
#define	DF_BME280_HUMIDITY_CALIB_DIG_H5_MSB				5
#define	DF_BME280_HUMIDITY_CALIB_DIG_H6					6
#define	DF_BME280_MASK_DIG_H4							0x0F

// calibration register address defines
#define DF_BME280_TEMPERATURE_CALIB_DIG_T1_LSB_REG      0x88
#define DF_BME280_TEMPERATURE_CALIB_DIG_T1_MSB_REG      0x89
#define DF_BME280_TEMPERATURE_CALIB_DIG_T2_LSB_REG      0x8A
#define DF_BME280_TEMPERATURE_CALIB_DIG_T2_MSB_REG      0x8B
#define DF_BME280_TEMPERATURE_CALIB_DIG_T3_LSB_REG      0x8C
#define DF_BME280_TEMPERATURE_CALIB_DIG_T3_MSB_REG      0x8D
#define DF_BME280_PRESSURE_CALIB_DIG_P1_LSB_REG         0x8E
#define DF_BME280_PRESSURE_CALIB_DIG_P1_MSB_REG         0x8F
#define DF_BME280_PRESSURE_CALIB_DIG_P2_LSB_REG         0x90
#define DF_BME280_PRESSURE_CALIB_DIG_P2_MSB_REG         0x91
#define DF_BME280_PRESSURE_CALIB_DIG_P3_LSB_REG         0x92
#define DF_BME280_PRESSURE_CALIB_DIG_P3_MSB_REG         0x93
#define DF_BME280_PRESSURE_CALIB_DIG_P4_LSB_REG         0x94
#define DF_BME280_PRESSURE_CALIB_DIG_P4_MSB_REG         0x95
#define DF_BME280_PRESSURE_CALIB_DIG_P5_LSB_REG         0x96
#define DF_BME280_PRESSURE_CALIB_DIG_P5_MSB_REG         0x97
#define DF_BME280_PRESSURE_CALIB_DIG_P6_LSB_REG         0x98
#define DF_BME280_PRESSURE_CALIB_DIG_P6_MSB_REG         0x99
#define DF_BME280_PRESSURE_CALIB_DIG_P7_LSB_REG         0x9A
#define DF_BME280_PRESSURE_CALIB_DIG_P7_MSB_REG         0x9B
#define DF_BME280_PRESSURE_CALIB_DIG_P8_LSB_REG         0x9C
#define DF_BME280_PRESSURE_CALIB_DIG_P8_MSB_REG         0x9D
#define DF_BME280_PRESSURE_CALIB_DIG_P9_LSB_REG         0x9E
#define DF_BME280_PRESSURE_CALIB_DIG_P9_MSB_REG         0x9F

#define DF_BME280_HUMIDITY_CALIB_DIG_H1_REG             0xA1

#define DF_BME280_HUMIDITY_CALIB_DIG_H2_LSB_REG         0xE1
#define DF_BME280_HUMIDITY_CALIB_DIG_H2_MSB_REG         0xE2
#define DF_BME280_HUMIDITY_CALIB_DIG_H3_REG             0xE3
#define DF_BME280_HUMIDITY_CALIB_DIG_H4_MSB_REG         0xE4
#define DF_BME280_HUMIDITY_CALIB_DIG_H4_LSB_REG         0xE5
#define DF_BME280_HUMIDITY_CALIB_DIG_H5_MSB_REG         0xE6
#define DF_BME280_HUMIDITY_CALIB_DIG_H6_REG             0xE7

// register address defines
#define DF_BME280_CHIP_ID_REG                           0xD0  /*Chip ID Register */
#define DF_BME280_RST_REG                               0xE0  /*Softreset Register */
#define DF_BME280_STAT_REG                              0xF3  /*Status Register */
#define DF_BME280_CTRL_MEAS_REG                         0xF4  /*Ctrl Measure Register */
#define DF_BME280_CTRL_HUMIDITY_REG                     0xF2  /*Ctrl Humidity Register*/
#define DF_BME280_CONFIG_REG                            0xF5  /*Configuration Register */
#define DF_BME280_PRESSURE_MSB_REG                      0xF7  /*Pressure MSB Register */
#define DF_BME280_PRESSURE_LSB_REG                      0xF8  /*Pressure LSB Register */
#define DF_BME280_PRESSURE_XLSB_REG                     0xF9  /*Pressure XLSB Register */
#define DF_BME280_TEMPERATURE_MSB_REG                   0xFA  /*Temperature MSB Reg */
#define DF_BME280_TEMPERATURE_LSB_REG                   0xFB  /*Temperature LSB Reg */
#define DF_BME280_TEMPERATURE_XLSB_REG                  0xFC  /*Temperature XLSB Reg */
#define DF_BME280_HUMIDITY_MSB_REG                      0xFD  /*Humidity MSB Reg */
#define DF_BME280_HUMIDITY_LSB_REG                      0xFE  /*Humidity LSB Reg */

// bit mask, length and position defines
#define DF_BME280_STAT_REG_MEASURING__POS               3
#define DF_BME280_STAT_REG_MEASURING__MSK               0x08
#define DF_BME280_STAT_REG_MEASURING__LEN               1
#define DF_BME280_STAT_REG_MEASURING__REG               DF_BME280_STAT_REG

#define DF_BME280_STAT_REG_IM_UPDATE__POS               0
#define DF_BME280_STAT_REG_IM_UPDATE__MSK               0x01
#define DF_BME280_STAT_REG_IM_UPDATE__LEN               1
#define DF_BME280_STAT_REG_IM_UPDATE__REG               DF_BME280_STAT_REG

// bit mask, length and position defines for temperature oversampling
#define DF_BME280_CTRL_MEAS_REG_OVERSAMP_TEMP__POS	    5
#define DF_BME280_CTRL_MEAS_REG_OVERSAMP_TEMP__MSK	    0xE0
#define DF_BME280_CTRL_MEAS_REG_OVERSAMP_TEMP__LEN	    3
#define DF_BME280_CTRL_MEAS_REG_OVERSAMP_TEMP__REG	    DF_BME280_CTRL_MEAS_REG

// bit mask, length and position defines for pressure oversampling
#define DF_BME280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__POS  2
#define DF_BME280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__MSK  0x1C
#define DF_BME280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__LEN  3
#define DF_BME280_CTRL_MEAS_REG_OVERSAMP_PRESSURE__REG  DF_BME280_CTRL_MEAS_REG

// bit mask, length and position defines for power mode
#define DF_BME280_CTRL_MEAS_REG_POWER_MODE__POS         0
#define DF_BME280_CTRL_MEAS_REG_POWER_MODE__MSK         0x03
#define DF_BME280_CTRL_MEAS_REG_POWER_MODE__LEN         2
#define DF_BME280_CTRL_MEAS_REG_POWER_MODE__REG         DF_BME280_CTRL_MEAS_REG

// bit mask, length and position defines for humidity oversampling
#define DF_BME280_CTRL_HUMID_REG_OVERSAMP_HUMID__POS    0
#define DF_BME280_CTRL_HUMID_REG_OVERSAMP_HUMID__MSK    0x07
#define DF_BME280_CTRL_HUMID_REG_OVERSAMP_HUMID__LEN    3
#define DF_BME280_CTRL_HUMID_REG_OVERSAMP_HUMID__REG   	DF_BME280_CTRL_HUMIDITY_REG

// bit mask, length and position defines for standby time
#define DF_BME280_CONFIG_REG_TSB__POS                   5
#define DF_BME280_CONFIG_REG_TSB__MSK                   0xE0
#define DF_BME280_CONFIG_REG_TSB__LEN                   3
#define DF_BME280_CONFIG_REG_TSB__REG                   DF_BME280_CONFIG_REG

// bit mask, length and position defines for filter
#define DF_BME280_CONFIG_REG_FILTER__POS                2
#define DF_BME280_CONFIG_REG_FILTER__MSK                0x1C
#define DF_BME280_CONFIG_REG_FILTER__LEN                3
#define DF_BME280_CONFIG_REG_FILTER__REG                DF_BME280_CONFIG_REG

// bit mask, length and position defines for spi enable
#define DF_BME280_CONFIG_REG_SPI3_ENABLE__POS           0
#define DF_BME280_CONFIG_REG_SPI3_ENABLE__MSK           0x01
#define DF_BME280_CONFIG_REG_SPI3_ENABLE__LEN           1
#define DF_BME280_CONFIG_REG_SPI3_ENABLE__REG           DF_BME280_CONFIG_REG

// bit mask, length and position defines for pressure   and temperature data
#define DF_BME280_PRESSURE_XLSB_REG_DATA__POS           4
#define DF_BME280_PRESSURE_XLSB_REG_DATA__MSK           0xF0
#define DF_BME280_PRESSURE_XLSB_REG_DATA__LEN           4
#define DF_BME280_PRESSURE_XLSB_REG_DATA__REG           DF_BME280_PRESSURE_XLSB_REG

#define DF_BME280_TEMPERATURE_XLSB_REG_DATA__POS        4
#define DF_BME280_TEMPERATURE_XLSB_REG_DATA__MSK        0xF0
#define DF_BME280_TEMPERATURE_XLSB_REG_DATA__LEN        4
#define DF_BME280_TEMPERATURE_XLSB_REG_DATA__REG        DF_BME280_TEMPERATURE_XLSB_REG

/*----------------------------------------------------------------------------*/
/* global variables */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* forward declarations */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* implementations */
/*----------------------------------------------------------------------------*/
BME280::BME280(uint8_t auBusNumber, uint16_t auDeviceId):
I2CDevice(auBusNumber, auDeviceId)
{

}

BME280::~BME280(void)
{

}

EError BME280::Init(void)
{
	EError eError;

	eError = this->Open();

	if (eError == 0)
	{
		eError = this->ReadRegisters(DF_BME280_CHIP_ID_REG, DF_BME280_GEN_READ_WRITE_DATA_LENGTH, &m_uChipId);
	}
	else
	{
		perror("BME280: Could not open I2C device.\n");
	}

	if(eError == 0)
	{
		eError = this->GetAllCalibrationValues();
		if(eError != 0)
		{
			perror("BME280: Could not get calibration values.\n");
		}
	}
	else
	{
		perror("BME280: Could not read chip ID.\n");
	}

	return eError;
}

EError BME280::GetAllCalibrationValues(void)
{
	uint8_t yuCalibParams[DF_BME280_CALIB_DATA_SIZE] = {0};
	// Get calibration parameters for pressure and temperature, stored at register 0x88..0xA1
	EError eError = this->ReadRegisters(DF_BME280_TEMPERATURE_CALIB_DIG_T1_LSB_REG,
											DF_BME280_PRESS_TEMP_CALIB_DATA_LENGTH, yuCalibParams);
	if(eError == eErrOk)
	{
		// Assign read registers to internal calibration parameter structure
		m_sCalibParams.m_uDigT1 = ((uint16_t)(((uint16_t)yuCalibParams[DF_BME280_TEMPERATURE_CALIB_DIG_T1_MSB]) << 8U)
								| yuCalibParams[DF_BME280_TEMPERATURE_CALIB_DIG_T1_LSB]);

		m_sCalibParams.m_iDigT2 = ((int16_t)(((int16_t)yuCalibParams[DF_BME280_TEMPERATURE_CALIB_DIG_T2_MSB]) << 8U)
								| yuCalibParams[DF_BME280_TEMPERATURE_CALIB_DIG_T2_LSB]);

		m_sCalibParams.m_iDigT3 = ((int16_t)(((int16_t)yuCalibParams[DF_BME280_TEMPERATURE_CALIB_DIG_T3_MSB]) << 8U)
								| yuCalibParams[DF_BME280_TEMPERATURE_CALIB_DIG_T3_LSB]);

		m_sCalibParams.m_uDigP1 = ((uint16_t)(((uint16_t)yuCalibParams[DF_BME280_PRESSURE_CALIB_DIG_P1_MSB]) << 8U)
								| yuCalibParams[DF_BME280_PRESSURE_CALIB_DIG_P1_LSB]);

		m_sCalibParams.m_iDigP2 = ((int16_t)(((int16_t)yuCalibParams[DF_BME280_PRESSURE_CALIB_DIG_P2_MSB]) << 8U)
								| yuCalibParams[DF_BME280_PRESSURE_CALIB_DIG_P2_LSB]);

		m_sCalibParams.m_iDigP3 = ((int16_t)(((int16_t)yuCalibParams[DF_BME280_PRESSURE_CALIB_DIG_P3_MSB]) << 8U)
								| yuCalibParams[DF_BME280_PRESSURE_CALIB_DIG_P3_LSB]);

		m_sCalibParams.m_iDigP4 = ((int16_t)(((int16_t)yuCalibParams[DF_BME280_PRESSURE_CALIB_DIG_P4_MSB]) << 8U)
								| yuCalibParams[DF_BME280_PRESSURE_CALIB_DIG_P4_LSB]);

		m_sCalibParams.m_iDigP5 = ((int16_t)(((int16_t)yuCalibParams[DF_BME280_PRESSURE_CALIB_DIG_P5_MSB]) << 8U)
								| yuCalibParams[DF_BME280_PRESSURE_CALIB_DIG_P5_LSB]);

		m_sCalibParams.m_iDigP6 = ((int16_t)(((int16_t)yuCalibParams[DF_BME280_PRESSURE_CALIB_DIG_P6_MSB]) << 8U)
								| yuCalibParams[DF_BME280_PRESSURE_CALIB_DIG_P6_LSB]);

		m_sCalibParams.m_iDigP7 = ((int16_t)(((int16_t)yuCalibParams[DF_BME280_PRESSURE_CALIB_DIG_P7_MSB]) << 8U)
								| yuCalibParams[DF_BME280_PRESSURE_CALIB_DIG_P7_LSB]);

		m_sCalibParams.m_iDigP8 = ((int16_t)(((int16_t)yuCalibParams[DF_BME280_PRESSURE_CALIB_DIG_P8_MSB]) << 8U)
								| yuCalibParams[DF_BME280_PRESSURE_CALIB_DIG_P8_LSB]);

		m_sCalibParams.m_iDigP9 = ((int16_t)(((int16_t)yuCalibParams[DF_BME280_PRESSURE_CALIB_DIG_P9_MSB]) << 8U)
								| yuCalibParams[DF_BME280_PRESSURE_CALIB_DIG_P9_LSB]);

		// dig_H1 register is part of the Pressure/Temperature block, at address 0xA1
		m_sCalibParams.m_uDigH1 = yuCalibParams[DF_BME280_HUMIDITY_CALIB_DIG_H1];

		// Get calibration parameters for pressure and temperature, stored at register 0x88..0xA1
		eError = this->ReadRegisters(DF_BME280_HUMIDITY_CALIB_DIG_H2_LSB_REG,
										DF_BME280_HUMIDITY_CALIB_DATA_LENGTH, yuCalibParams);

		if(eError == eErrOk)
		{
			// Assign read registers to internal calibration parameter structure
			m_sCalibParams.m_iDigH2 = ((int16_t)(((int16_t)yuCalibParams[DF_BME280_HUMIDITY_CALIB_DIG_H2_MSB]) << 8U)
									| yuCalibParams[DF_BME280_HUMIDITY_CALIB_DIG_H2_LSB]);

			m_sCalibParams.m_uDigH3 = yuCalibParams[DF_BME280_HUMIDITY_CALIB_DIG_H3];

			m_sCalibParams.m_iDigH4 = ((int16_t)(((int16_t)yuCalibParams[DF_BME280_HUMIDITY_CALIB_DIG_H4_MSB]) << 4U)
									| (DF_BME280_MASK_DIG_H4 & yuCalibParams[DF_BME280_HUMIDITY_CALIB_DIG_H4_LSB]));

			m_sCalibParams.m_iDigH5 = ((int16_t)(((int16_t)yuCalibParams[DF_BME280_HUMIDITY_CALIB_DIG_H5_MSB]) << 4U)
									| (yuCalibParams[DF_BME280_HUMIDITY_CALIB_DIG_H4_LSB] >> 4U));

			m_sCalibParams.m_iDigH6 = (int16_t)(yuCalibParams[DF_BME280_HUMIDITY_CALIB_DIG_H6]);
		}
	}

	return eError;
}

EError BME280::ReadAllValuesUncomp(void)
{
	EError eError = this->ReadUncompTemperature();

	if(eError == eErrOk)
	{
		eError = this->ReadUncompPressure();
	}

	if(eError == eErrOk)
	{
		eError = this->ReadUncompHumidity();
	}

	return eError;
}

EError BME280::CompensateAllValues(void)
{
	EError eError = this->CompensateTemperature();

	if(eError == eErrOk)
	{
		eError = this->CompensatePressure();
	}

	if(eError == eErrOk)
	{
		eError = this->CompensateHumidity();
	}

	return eError;
}

EError BME280::ReadUncompTemperature(void)
{
	EError eError = eErrOk;

	/* uTempData[0] - Temperature MSB
	 * uTempData[1] - Temperature LSB
	 * uTempData[2] - Temperature xLSB
	 */
	uint8_t uTempData[DF_BME280_TEMPERATURE_DATA_SIZE] = {0};

	eError = this->ReadRegisters(DF_BME280_TEMPERATURE_MSB_REG, DF_BME280_TEMPERATURE_DATA_LENGTH, uTempData);

	if(eError == eErrOk)
	{
		m_iTempUncomp = (int32_t)((((uint32_t)uTempData[DF_BME280_TEMPERATURE_MSB_DATA]) << 12U)
								| (((uint32_t)uTempData[DF_BME280_TEMPERATURE_LSB_DATA]) << 4U)
								| (((uint32_t)uTempData[DF_BME280_TEMPERATURE_XLSB_DATA]) >> 4U));
	}

	return eError;
}

EError BME280::CompensateTemperature(void)
{
	EError eError = eErrOk;
	return eError;
}

void BME280::CompensateTemperatureI32(void)
{
	int32_t iTempVal1 = 0;
	int32_t iTempVal2 = 0;

	/* calculate temp value 1 */
	iTempVal1 = ((((m_iTempUncomp >> 3U) - ((int32_t)m_sCalibParams.m_uDigT1 << 1U))) * ((int32_t)m_sCalibParams.m_iDigT2)) >> 11U;
	/* calculate temp value 2 */
	iTempVal2 = (((((m_iTempUncomp >> 4U) - ((int32_t)m_sCalibParams.m_uDigT1))
			* ((m_iTempUncomp >> 4U) - ((int32_t)m_sCalibParams.m_uDigT1)))
			>> 12U) * ((int32_t) m_sCalibParams.m_iDigT3)) >> 14U;
	/* calculate t_fine */
	m_sCalibParams.m_iTFine = iTempVal1 + iTempVal2;
	/* calculate temperature */
	m_iTempComp = (m_sCalibParams.m_iTFine * 5 + 128) >> 8;
}

EError BME280::CompensateTemperatureDouble(void)
{
	EError eError = eErrOk;
	return eError;
}

EError BME280::ReadUncompPressure(void)
{
	EError eError = eErrOk;

	/* uPressData[0] - Pressure MSB
	 * uPressData[1] - Pressure LSB
	 * uPressData[2] - Pressure xLSB
	 */
	uint8_t uPressData[DF_BME280_PRESSURE_DATA_SIZE] = {0};

	eError = this->ReadRegisters(DF_BME280_TEMPERATURE_MSB_REG, DF_BME280_TEMPERATURE_DATA_LENGTH, uPressData);

	if(eError == eErrOk)
	{
		m_uPressUncomp = (uint32_t)((((uint32_t)uPressData[DF_BME280_PRESSURE_MSB_DATA]) << 12U)
								| (((uint32_t)uPressData[DF_BME280_PRESSURE_LSB_DATA]) << 4U)
								| (((uint32_t)uPressData[DF_BME280_PRESSURE_XLSB_DATA]) >> 4U));
	}

	return eError;
}

EError BME280::CompensatePressure(void)
{
	EError eError = eErrOk;
	return eError;
}

EError BME280::CompensatePressureU32(void)
{
	EError eError = eErrOk;
	int32_t iTempVal1;
	int32_t iTempVal2;

	/* calculate temp value 1 */
	iTempVal1 = (((int32_t)m_sCalibParams.m_iTFine) >> 1U) - ((int32_t)64000);
	/* calculate temp value 2 */
	iTempVal2 = (((iTempVal1 >> 2U) * (iTempVal1 >> 2U)) >> 11U) * ((int32_t)m_sCalibParams.m_iDigP6);
	iTempVal2 =  iTempVal2 + ((iTempVal1 * ((int32_t)m_sCalibParams.m_iDigP5)) << 1U);
	iTempVal2 = (iTempVal2 >> 2U) + (((int32_t)m_sCalibParams.m_iDigP4) << 16U);
	/* calculate temp value 1 */
	iTempVal1 = (((m_sCalibParams.m_iDigP3 * (((iTempVal1 >> 2U) * (iTempVal1 >> 2U)) >> 13U)) >> 3U) +
			((((int32_t)m_sCalibParams.m_iDigP2) * iTempVal1) >> 1U)) >> 18U;
	iTempVal1 = (((32768 + iTempVal1)) * ((int32_t)m_sCalibParams.m_uDigP1)) >> 15;
	/* calculate pressure */
	m_uPressComp = (((uint32_t)(((int32_t)1048576) - m_uPressUncomp) - (iTempVal2 >> 12))) * 3125;

	if(m_uPressComp < 0x80000000)
	{
		/* avoid division by zero */
		if(iTempVal1 != 0)
		{
			m_uPressComp = (m_uPressComp << 1U) / ((uint32_t)iTempVal1);
		}
		else
		{
			eError = eErrBME280PressCompDiv0;
		}
	}
	else
	{
		/* avoid division by zero */
		if(iTempVal1 != 0)
		{
			m_uPressComp = (m_uPressComp / ((uint32_t)iTempVal1)) * 2U;
		}
		else
		{
			eError = eErrBME280PressCompDiv0;
		}
	}

	if(eError == eErrOk)
	{
		iTempVal1 = (((int32_t)m_sCalibParams.m_iDigP9) * ((int32_t)(((m_uPressComp >> 3U) * (m_uPressComp >> 3U)) >> 13U))) >> 12U;
		iTempVal2 = (((int32_t)(m_uPressComp >> 2U)) * ((int32_t)m_sCalibParams.m_iDigP8)) >> 13U;

		m_uPressComp = (uint32_t)((int32_t)m_uPressComp + ((iTempVal1 + iTempVal2 + m_sCalibParams.m_iDigP7) >> 4U));
	}

	return eError;
}

EError BME280::CompensatePressureDouble(void)
{
	EError eError = eErrOk;
	return eError;
}

EError BME280::ReadUncompHumidity(void)
{
	EError eError = eErrOk;

	/* uHumidData[0] - Humidity MSB
	 * uHumidData[1] - Humidity LSB
	 */
	uint8_t uHumidData[DF_BME280_HUMIDITY_DATA_SIZE] = {0};

	eError = this->ReadRegisters(DF_BME280_TEMPERATURE_MSB_REG, DF_BME280_TEMPERATURE_DATA_LENGTH, uHumidData);

	if(eError == eErrOk)
	{
		m_iHumidUncomp = (int32_t)((((uint32_t)uHumidData[DF_BME280_HUMIDITY_MSB_DATA]) << 8U)
								| ((uint32_t)uHumidData[DF_BME280_HUMIDITY_LSB_DATA]));
	}

	return eError;
}

EError BME280::CompensateHumidity(void)
{
	EError eError = eErrOk;
	return eError;
}

EError BME280::CompensateHumidityU32(void)
{

}

EError BME280::CompensateHumidityDouble(void)
{
	EError eError = eErrOk;
	return eError;
}
