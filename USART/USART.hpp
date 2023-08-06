/*
 * USART.hpp
 *
 *  Created on: Jul 25, 2023
 *      Author: Kunal
 */

#ifndef USART_HPP_
#define USART_HPP_


#include "main.h"
#include "GPIO.hpp"
#include "DMA.hpp"


class USART
{
public:


	enum class Data_Word_Length
	{
		Bit8 = 0 << 12,
		Bit9 = 1 << 12,
	}Data_Word_Length;

	enum class Parity
	{
		Disable = -1,
		Enable = 1,
		Even_Parity = 2,
		Odd_Parity  = 3,
	}Parity;

	enum class Stop_Bits
	{
		Bit1   = 1,
		Bit0_5 = 2,
		Bit2   = 3,
		Bit1_5 = 4,
	}Stop_Bits;

	enum class Clock_Polarity
	{
		Steady_Low = 1,
		Steady_High  = 2,
	}Clock_Polarity;

	enum class Clock_Phase
	{
		First_Clock_Transition    = 1,
		Seecond_Clock_Transition  = 2,
	}Clock_Phase;


	enum class Interrupt
	{
		Disable = -1,
		Idle = 1,
		Receiver_Not_Empty = 2,
		Transmit_Data_Register_Empty = 3,
		Transmiter_Complete = 4,
		Parity_Error = 5,
	}Interrupt;

	enum class DMA_Setup
	{
		Disable_DMA = 0,
		Transmit_Only = 1,
		Receive_Only = 2,
		Both_Transmit_Receive = 3,
	}DMA_Setup;



	struct Config
	{
		USART_TypeDef *Port;
		//Asynchronous + Synchronous
		bool pin_remap = DISABLE;
		bool low_power = DISABLE;
		uint32_t baudrate = 9600;
		uint8_t data_word_length = static_cast<uint8_t>(Data_Word_Length::Bit8);
		uint8_t stop_bits = static_cast<uint8_t>(Stop_Bits:: Bit1);
		int8_t parity = static_cast<uint8_t>(Parity::Disable);
		uint8_t interrupt = static_cast<uint8_t>(Interrupt::Disable);
		uint8_t dma_setup = static_cast<uint8_t>(DMA_Setup::Disable);
		DMA USART_DMA;
		//Only Synchronous
		bool clock_enable = DISABLE;
		uint8_t clock_polarity = static_cast<uint8_t>(Clock_Polarity::Steady_Low);
		uint8_t clock_phase = static_cast<uint8_t>(Clock_Phase::First_Clock_Transition);
		uint8_t node_address = 0x00;
	}Config;

	void Reset_Config(void);
	int8_t Init(void);
	int8_t DeInit(void);
	void Transmit_Byte(int data);
	int Receive_Byte(void);
	void Transmit_Buffer(void *transmit_buffer_address, uint16_t buffer_length);
	void Receive_Buffer(void *receive_buffer_address, uint16_t buffer_length);




private:

	enum class error_codes
	{
		ALL_OK = 1,
		Peripheral_Clock = -1,
		Low_power_for_UART_4n5 = -2,
	}error_codes;

};


#endif /* USART_HPP_ */
