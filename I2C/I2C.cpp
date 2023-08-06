/*
 * I2C.cpp
 *
 *  Created on: Jul 14, 2023
 *      Author: Kunal
 */


#include "I2C.hpp"

GPIO I2C_SCL;
GPIO I2C_SDA;


void I2C::Init()
{
	if(I2C::Config.PORT == I2C1)
	{
		RCC -> APB1ENR |= RCC_APB1ENR_I2C1EN;

		if(I2C::Config.remap == false)
		{
			I2C_SCL.Config.PORT = GPIOB;
			I2C_SCL.Config.Pin = 6;
			I2C_SCL.Config.alternate_function = (int)GPIO::Alternate_Function::I2C_1_2_3;
			I2C_SCL.Config.mode = (int)GPIO::Mode::Alternate;
			I2C_SCL.Config.pull = (int)GPIO::Pull::UP;
			I2C_SCL.Config.output_type = (int)GPIO::Output_Type::Open_Drain;
			I2C_SCL.Config.speed = (int)GPIO::Speed::Very_High_Speed;
			I2C_SCL.Init();

			I2C_SDA.Config.PORT = GPIOB;
			I2C_SDA.Config.Pin = 7;
			I2C_SDA.Config.alternate_function = (int)GPIO::Alternate_Function::I2C_1_2_3;
			I2C_SDA.Config.mode = (int)GPIO::Mode::Alternate;
			I2C_SDA.Config.pull = (int)GPIO::Pull::UP;
			I2C_SDA.Config.output_type = (int)GPIO::Output_Type::Open_Drain;
			I2C_SDA.Config.speed = (int)GPIO::Speed::Very_High_Speed;
			I2C_SDA.Init();
		}
		else
		{
			I2C_SCL.Config.PORT = GPIOB;
			I2C_SCL.Config.Pin = 8;
			I2C_SCL.Config.alternate_function = (int)GPIO::Alternate_Function::I2C_1_2_3;
			I2C_SCL.Config.mode = (int)GPIO::Mode::Alternate;
			I2C_SCL.Config.pull = (int)GPIO::Pull::UP;
			I2C_SCL.Config.output_type = (int)GPIO::Output_Type::Open_Drain;
			I2C_SCL.Config.speed = (int)GPIO::Speed::Very_High_Speed;
			I2C_SCL.Init();

			I2C_SDA.Config.PORT = GPIOB;
			I2C_SDA.Config.Pin = 9;
			I2C_SDA.Config.alternate_function = (int)GPIO::Alternate_Function::I2C_1_2_3;
			I2C_SDA.Config.mode = (int)GPIO::Mode::Alternate;
			I2C_SDA.Config.pull = (int)GPIO::Pull::UP;
			I2C_SDA.Config.output_type = (int)GPIO::Output_Type::Open_Drain;
			I2C_SDA.Config.speed = (int)GPIO::Speed::Very_High_Speed;
			I2C_SDA.Init();
		}
	}

	if(I2C::Config.PORT == I2C2)
	{
		RCC -> APB1ENR |= RCC_APB1ENR_I2C2EN;

		if(I2C::Config.remap == false)
		{
			I2C_SCL.Config.PORT = GPIOB;
			I2C_SCL.Config.Pin = 6;
			I2C_SCL.Config.alternate_function = (int)GPIO::Alternate_Function::I2C_1_2_3;
			I2C_SCL.Config.mode = (int)GPIO::Mode::Alternate;
			I2C_SCL.Config.pull = (int)GPIO::Pull::UP;
			I2C_SCL.Config.output_type = (int)GPIO::Output_Type::Open_Drain;
			I2C_SCL.Config.speed = (int)GPIO::Speed::Very_High_Speed;
			I2C_SCL.Init();

			I2C_SDA.Config.PORT = GPIOB;
			I2C_SDA.Config.Pin = 7;
			I2C_SDA.Config.alternate_function = (int)GPIO::Alternate_Function::I2C_1_2_3;
			I2C_SDA.Config.mode = (int)GPIO::Mode::Alternate;
			I2C_SDA.Config.pull = (int)GPIO::Pull::UP;
			I2C_SDA.Config.output_type = (int)GPIO::Output_Type::Open_Drain;
			I2C_SDA.Config.speed = (int)GPIO::Speed::Very_High_Speed;
			I2C_SDA.Init();
		}
		else
		{
			I2C_SCL.Config.PORT = GPIOB;
			I2C_SCL.Config.Pin = 8;
			I2C_SCL.Config.alternate_function = (int)GPIO::Alternate_Function::I2C_1_2_3;
			I2C_SCL.Config.mode = (int)GPIO::Mode::Alternate;
			I2C_SCL.Config.pull = (int)GPIO::Pull::UP;
			I2C_SCL.Config.output_type = (int)GPIO::Output_Type::Open_Drain;
			I2C_SCL.Config.speed = (int)GPIO::Speed::Very_High_Speed;
			I2C_SCL.Init();

			I2C_SDA.Config.PORT = GPIOB;
			I2C_SDA.Config.Pin = 9;
			I2C_SDA.Config.alternate_function = (int)GPIO::Alternate_Function::I2C_1_2_3;
			I2C_SDA.Config.mode = (int)GPIO::Mode::Alternate;
			I2C_SDA.Config.pull = (int)GPIO::Pull::UP;
			I2C_SDA.Config.output_type = (int)GPIO::Output_Type::Open_Drain;
			I2C_SDA.Config.speed = (int)GPIO::Speed::Very_High_Speed;
			I2C_SDA.Init();
		}
	}


	I2C::Config.PORT -> CR1 &= ~I2C_CR1_PE;
	I2C::Config.PORT -> CR1 |= I2C_CR1_SWRST;
	I2C::Config.PORT -> CR1 &= ~I2C_CR1_SWRST;
	I2C::Config.PORT -> CR2 |= 48;
	I2C::Config.PORT -> CCR |= (I2C_CCR_FS | (I2C_CCR_CCR & (SystemCoreClock / (2 * I2C::Config.frequency))));
	I2C::Config.PORT -> TRISE = ((SystemCoreClock / I2C::Config.frequency) + 1);
	I2C::Config.PORT -> CR1 |= I2C_CR1_NOSTRETCH;
	I2C::Config.PORT -> CR1 |= I2C_CR1_PE;
}


void I2C::Start()
{
	I2C::Config.PORT -> CR1 |= I2C_CR1_START;
	while(!(I2C::Config.PORT -> SR1 & I2C_SR1_SB)){}
}

void I2C::Send_Address(uint8_t Address, bool read_write)
{
	int temp = 0;
	I2C::Config.PORT -> DR = Address << 1  | read_write;
	while((I2C::Config.PORT -> SR1 & 2) == 0){}
	while((I2C::Config.PORT -> SR1 & 2))
	{
		temp = I2C::Config.PORT -> SR1;
		temp = I2C::Config.PORT -> SR2;
		if((I2C::Config.PORT -> SR1 & 2) == 0)
		{
			break;
		}
	}
}

void I2C::Send_Byte(uint8_t byte)
{
	while((I2C::Config.PORT -> SR1 & 0x80) == 0){}
	I2C::Config.PORT -> DR = byte;
	while((I2C::Config.PORT -> SR1 & 0x80) == 0){}
}

uint16_t I2C::Receive_Byte()
{
	volatile int temp;
	I2C::Config.PORT -> CR1 |= I2C_CR1_ACK;
	while((I2C::Config.PORT -> SR1 & I2C_SR1_RXNE) == 0){}
	temp = I2C::Config.PORT -> DR;
	I2C::Config.PORT -> CR1 &= ~I2C_CR1_ACK;
	return temp;
}

void I2C::Stop()
{
	I2C::Config.PORT -> CR1 |= I2C_CR1_STOP;
}


void I2C::Send_ACK()
{
	I2C::Config.PORT -> CR1 |= I2C_CR1_ACK;
}

void I2C::Send_NACK()
{
	I2C::Config.PORT -> CR1 &= ~I2C_CR1_ACK;
}


void I2C::Write_Register(uint8_t register_address, uint8_t data)
{
	Start();
	Send_Address(register_address, 0);
	Send_Byte(register_address);
	Send_Byte(data);
	Stop();
}


uint16_t I2C::Read_Register(uint8_t register_address)
{
	volatile int temp;

	Start();
	Send_Address(register_address, 0);
	Send_Byte(register_address);
	Stop();
	Start();
	Send_Address(register_address, 1);
	temp = Receive_Byte();
	Stop();

	return temp;
}

