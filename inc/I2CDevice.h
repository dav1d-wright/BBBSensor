/**----------------------------------------------------------------------------
	\file		I2CDevice.h
	\author		D. Wright
	\version	0

	\brief Class that implements a generic I2C Device on an embedded linux platform.

	Long comment on this file.

  ----------------------------------------------------------------------------*/

#ifndef INC_I2CDEVICE_H_
#define INC_I2CDEVICE_H_

/*----------------------------------------------------------------------------*/

/*!	\defgroup Devices Higher functionality devices.
	\{
*/

/*!	\defgroup DEV_I2C I2C Device (I2C)
 	 * \brief The I2C device handles the I2C communication from the BeagleBone master to the slave device.
	\{
*/

class I2CDevice
{
private:
	uint8_t m_uBusNumber;
	uint8_t m_uDeviceId;
	int32_t m_iDeviceFile;

public:
	I2CDevice(uint8_t auBusNumber, uint16_t auDeviceId);
	~I2CDevice(void);
	/*!	\brief Open I2C bus.
		\param None.
		\return Error code, \see \ref EError.
	*/
	virtual EError Open(void);
	/*!	\brief Close I2C bus.
		\param None.
		\retval None.
	*/
	virtual void Close(void);
	virtual EError Write(uint8_t auValue);
	virtual EError WriteRegister(uint8_t auRegisterAddress, uint8_t auValue);
	virtual EError ReadRegister(uint8_t auRegisterAddress, uint8_t* apuReadValue);
	virtual EError ReadRegisters(uint8_t auRegisterAddress, uint8_t auNumRegisters, uint8_t* apuReadValues);
	virtual EError DumpRegisters(uint8_t auNumRegisters);
};


#endif /* INC_I2CDEVICE_H_ */
