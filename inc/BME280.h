/*
 * BME280.h
 *
 *  Created on: 10 May 2016
 *      Author: David Wright
 */

#ifndef INC_BME280_H_
#define INC_BME280_H_
class BME280: public I2CDevice
{
private:
public:
	BME280(uint8_t auBusNumber, uint16_t auDeviceId);
	~BME280(void);
};


#endif /* INC_BME280_H_ */
