/*
 * I2C.hpp
 *
 *  Created on: Jul 14, 2023
 *      Author: Kunal
 */

#ifndef I2C_HPP_
#define I2C_HPP_


#include "main.h"
#include "GPIO.hpp"



class I2C
{
public:

	struct Config
	{
		I2C_TypeDef *PORT;
		uint32_t frequency = 400000;
		bool remap = false;
		bool DMA_RX = false;
		bool DMA_TX = false;
		uint8_t slave_device_address;
	}Config;


	void Init(void);
	void Start(void);
	void Send_Address(uint8_t Address, bool read_write);
	void Send_Byte(uint8_t byte);
	uint16_t Receive_Byte(void);
	void Send_ACK();
	void Send_NACK();
	void Stop();
	void Write_Register(uint8_t register_address, uint8_t data);
	uint16_t Read_Register(uint8_t register_address);



private:
};



#endif /* I2C_HPP_ */
