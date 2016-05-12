/*
 * Errors.h
 *
 *  Created on: 11 May 2016
 *      Author: David Wright
 */

#ifndef INC_ERRORS_H_
#define INC_ERRORS_H_

typedef enum
{
	eErrOk = 0,
	eErrI2CBusNumber,						/* Wrong I2C bus number */
	eErrI2CCannotOpenBus,					/* Cannot open I2C bus */
	eErrI2CCannotConnect,					/* Cannot connect to I2C device */
	eErrI2CCannotWrite,						/* Cannot write to I2C device */
	eErrI2CCannotRead,						/* Cannot read from I2C device */
	eErrBME280PressCompDiv0					/* Division by zero during pressure compensation */
}EError;




#endif /* INC_ERRORS_H_ */
