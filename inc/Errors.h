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
	/*! \brief Division by zero during pressure compensation */
	eErrBME280PressCompDiv0
}EError;




#endif /* INC_ERRORS_H_ */
