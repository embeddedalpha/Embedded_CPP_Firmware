/*
 * GPIO.cpp
 *
 *  Created on: Jul 14, 2023
 *      Author: Kunal
 */



#include "GPIO.hpp"



void GPIO::Init()
{
	if(GPIO::Config.PORT == GPIOA) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	if(GPIO::Config.PORT == GPIOB) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	if(GPIO::Config.PORT == GPIOC) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	if(GPIO::Config.PORT == GPIOD) RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	if(GPIO::Config.PORT == GPIOE) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	if(GPIO::Config.PORT == GPIOH) RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;

	GPIO::Config.PORT -> MODER  |=  static_cast<uint32_t>(GPIO::Config.mode) << (GPIO::Config.Pin * 2);
	GPIO::Config.PORT -> OTYPER |=  static_cast<uint32_t>(GPIO::Config.output_type) << (GPIO::Config.Pin * 1);
	GPIO::Config.PORT -> OSPEEDR |=  static_cast<uint32_t>(GPIO::Config.speed) << (GPIO::Config.Pin * 2);
	GPIO::Config.PORT -> PUPDR |=  static_cast<uint32_t>(GPIO::Config.pull) << (GPIO::Config.Pin * 2);

	if(GPIO::Config.Pin  < 8) GPIO::Config.PORT -> AFR[0] |=  static_cast<uint32_t>(GPIO::Config.alternate_function) << (GPIO::Config.Pin * 4);
	if(GPIO::Config.Pin >= 8) GPIO::Config.PORT -> AFR[1] |=  static_cast<uint32_t>(GPIO::Config.alternate_function) << (GPIO::Config.Pin * 4);

}

void GPIO::Write(bool bit)
{
	if(bit) GPIO::Config.PORT -> BSRR |= GPIO_BSRR_BS0 << GPIO::Config.Pin;
	else    GPIO::Config.PORT -> BSRR |= GPIO_BSRR_BR0 << GPIO::Config.Pin;
}

int GPIO::Read()
{
	return ((GPIO::Config.PORT -> IDR) & (1UL << GPIO::Config.Pin)) != 0;
}

void GPIO::Interrupt_Setup()
{
	EXTI ->IMR |= 1 << GPIO::Config.Pin;
	switch (GPIO::Config.edge)
	{
		case 0:
			EXTI ->RTSR |= 1 << GPIO::Config.Pin;
			break;
		case 1:
			EXTI ->FTSR |= 1 << GPIO::Config.Pin;
			break;
		case 2:
			EXTI ->RTSR |= 1 << GPIO::Config.Pin;
			EXTI ->FTSR |= 1 << GPIO::Config.Pin;
			break;
	}

	if(GPIO::Config.Pin == 0)
	{
		NVIC_SetPriority(EXTI0_IRQn,0);
		NVIC_EnableIRQ(EXTI0_IRQn);
	}
	else if(GPIO::Config.Pin == 1)
	{
		NVIC_SetPriority(EXTI1_IRQn,0);
		NVIC_EnableIRQ(EXTI1_IRQn);
	}
	else if(GPIO::Config.Pin == 2)
	{
		NVIC_SetPriority(EXTI2_IRQn,0);
		NVIC_EnableIRQ(EXTI2_IRQn);
	}
	else if(GPIO::Config.Pin == 3)
	{
		NVIC_SetPriority(EXTI3_IRQn,0);
		NVIC_EnableIRQ(EXTI3_IRQn);
	}
	else if(GPIO::Config.Pin == 4)
	{
		NVIC_SetPriority(EXTI4_IRQn,0);
		NVIC_EnableIRQ(EXTI4_IRQn);
	}
	else if(GPIO::Config.Pin >= 5 || GPIO::Config.Pin <= 9)
	{
		NVIC_SetPriority(EXTI9_5_IRQn,0);
		NVIC_EnableIRQ(EXTI9_5_IRQn);
	}
	else if(GPIO::Config.Pin >= 10 || GPIO::Config.Pin <= 15)
	{
		NVIC_SetPriority(EXTI15_10_IRQn,0);
		NVIC_EnableIRQ(EXTI15_10_IRQn);
	}

}
