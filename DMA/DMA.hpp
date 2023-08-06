/*
 * DMA.hpp
 *
 *  Created on: Jul 15, 2023
 *      Author: Kunal
 */

#ifndef DMA_HPP_
#define DMA_HPP_

#include "main.h"

class DMA {
public:


	typedef struct DMA_Instance
	{
		DMA_TypeDef *controller;
		DMA_Stream_TypeDef *stream[2];
		uint8_t channel;
		bool availablity;
	}DMA_Instance;


//	DMA_Instance USART1_DMA_Instance = {.stream[0] = DMA2_Stream7, .channel = 4};

	DMA_Instance USART_DMA_Instance[6];










	enum class Interrupt
	{
		Disable = -1,
		Half_Transfer = 1,
		Transfer_Complete,
		Transfer_Error,
		FIFO_Overrun_Underrun,
		Direct_Mode_Error,
	}Interrupt;

	enum class Data_Size
	{
		bit8 = 0,
		bit16,
		bit32,
	}Data_Size;

	enum Direction{
		Peripheral_to_Memory	= 0,
		Memory_to_Peripheral	= 1,
		Memory_to_Memory		= 2,
	}Direction;

	enum class Priority
	{
		low = 0,
		medium,
		high,
		very_high,
	}Priority;

	enum class Flow_Controller
	{
		DMA_Flow_Controller = 1,
		Peripheral_Flow_Controller = 2,
	}Flow_Controller;

	DMA()
	{
		setup_DMA_Instance();
	}


	struct Config
	{
		DMA_TypeDef *Controller;
		DMA_Stream_TypeDef *Stream;
		uint8_t channel;
		bool circular_buffer;
		uint8_t interrupt = static_cast<uint8_t>(Interrupt::Disable);
		uint32_t *Desitnation_Address;
		uint32_t *Source_Address;
		uint8_t direction = static_cast<uint8_t>(Direction::Peripheral_to_Memory);
		bool memory_increment = DISABLE;
		bool peripheral_increment = DISABLE;
		uint8_t priority;
		uint8_t memory_data_size;
		uint8_t periphera_data_size;
		uint8_t flow_controller = static_cast<uint8_t>(Flow_Controller::DMA_Flow_Controller);
	}Config;



	void setup_DMA_Instance(void);

	void DeInit();
	int  Init();
	int  Trigger();


private:
};



#endif /* DMA_HPP_ */
