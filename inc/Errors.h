/**----------------------------------------------------------------------------
	\file		Errors.h
	\author		D. Wright
	\version	0

	\brief Global error codes.

	Long comment on this file.

  ----------------------------------------------------------------------------*/

#ifndef INC_ERRORS_H
#define INC_ERRORS_H


/*----------------------------------------------------------------------------*/

/*!	\defgroup Errors Global error code list.
	\brief Global list of error codes.

	\{
*/


/*----------------------------------------------------------------------------*/
/*!	\brief Error codes
*/
typedef enum
{
	/*!	\brief No error.*/
	eErrOk = 0,

	/*! \brief Wrong I2C bus number */
	eErrI2CBusNumber,
	/*! \brief Cannot open I2C bus */
	eErrI2CCannotOpenBus,
	/*! \brief Cannot connect to I2C device */
	eErrI2CCannotConnect,
	/*! \brief Cannot write to I2C device */
	eErrI2CCannotWrite,
	/*! \brief Cannot read from I2C device */
	eErrI2CCannotRead,
	/*! \brief BME280 init failed */
	eErrBME280Init,
	/*! \brief BME280 setting power mode failed */
	eErrBME280SetPowerMode,
	/*! \brief BME280 setting standby duration failed */
	eErrBME280SetStandbyDuration,
	/*! \brief BME280 setting humidity oversampling failed */
	eErrBME280SetOversampHumidity,
	/*! \brief BME280 setting pressure oversampling failed */
	eErrBME280SetOversampPressure,
	/*! \brief BME280 setting temperature oversampling failed */
	eErrBME280SetOversampTemperature,
	/*! \brief BME280 reading uncompensated humidity failed */
	eErrBME280ReadUncompHumidity,
	/*! \brief BME280 reading uncompensated pressure failed */
	eErrBME280ReadUncompPressure,
	/*! \brief BME280 reading uncompensated temperature failed */
	eErrBME280ReadUncompTemperature,
	/*! \brief Division by zero during pressure compensation */
	eErrBME280PressCompDiv0
}EError;




#endif /* INC_ERRORS_H_ */
