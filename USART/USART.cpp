/*
 * USART.cpp
 *
 *  Created on: Jul 25, 2023
 *      Author: Kunal
 */

#include "USART.hpp"

//DMA USART_DMA;


void USART::Reset_Config()
{
	Config.pin_remap = DISABLE;
	Config.low_power = DISABLE;
	Config.baudrate = 9600;
	Config.data_word_length = static_cast<uint8_t>(Data_Word_Length::Bit8);
	Config.stop_bits = static_cast<uint8_t>(Stop_Bits:: Bit1);
	Config.parity = static_cast<uint8_t>(Parity::Disable);
	Config.interrupt = static_cast<uint8_t>(Interrupt::Disable);
	Config.dma_enable = DISABLE;
	Config.clock_enable = DISABLE;
	Config.clock_polarity = static_cast<uint8_t>(Clock_Polarity::Steady_Low);
	Config.clock_phase = static_cast<uint8_t>(Clock_Phase::First_Clock_Transition);
	Config.node_address = 0x00;

}

int8_t USART::DeInit()
{
	if(Config.Port == USART1) {
		RCC -> APB2ENR   &= ~(RCC_APB2ENR_USART1EN);
		RCC -> APB2LPENR &= ~(RCC_APB2LPENR_USART1LPEN);
	}
	else if(Config.Port == USART2){
		RCC -> APB1ENR &= ~RCC_APB1ENR_USART2EN;
		RCC -> APB1LPENR &= ~(RCC_APB1LPENR_USART2LPEN);
	}
	else if(Config.Port == USART3){
		RCC -> APB1ENR &= ~RCC_APB1ENR_USART3EN;
		RCC -> APB1LPENR &= ~(RCC_APB1LPENR_USART3LPEN);
	}
	else if(Config.Port == UART4){
		RCC -> APB1ENR &= ~RCC_APB1ENR_UART4EN;
//		RCC -> APB1LPENR &= ~(RCC_APB1LPENR_UART4LPEN);
	}
	else if(Config.Port == UART5){
		RCC -> APB1ENR &= ~RCC_APB1ENR_UART5EN;
		RCC -> APB1LPENR &= ~(RCC_APB1LPENR_UART5LPEN);
	}
	else if(Config.Port == USART6){
		RCC -> APB2ENR &= ~RCC_APB2ENR_USART6EN;
		RCC -> APB2LPENR &= ~(RCC_APB2LPENR_USART6LPEN);
	}
	else {
		return static_cast<int8_t>(error_codes::Peripheral_Clock);
	}

	return static_cast<int8_t>(error_codes::ALL_OK);
}

int8_t USART::Init()
{
	uint32_t fck = 0;

	if(Config.Port == USART1)
	{
		if(Config.low_power) RCC -> APB2LPENR |= RCC_APB2LPENR_USART1LPEN;
		else RCC -> APB2ENR |= RCC_APB2ENR_USART1EN;

		fck = SystemCoreClock;
	}
	if(Config.Port == USART2)
	{
		fck = SystemCoreClock/2;
		if(Config.low_power) RCC -> APB1LPENR |= RCC_APB1LPENR_USART2LPEN;
		else RCC -> APB1ENR |= RCC_APB1ENR_USART2EN;
	}
	if(Config.Port == USART3)
	{
		fck = SystemCoreClock/2;
		if(Config.low_power) RCC -> APB1LPENR |= RCC_APB1LPENR_USART3LPEN;
		else RCC -> APB1ENR |= RCC_APB1ENR_USART3EN;
	}
	if(Config.Port == UART4)
	{
		fck = SystemCoreClock/2;
		if(Config.low_power) return static_cast<int8_t>(error_codes::Low_power_for_UART_4n5);
		RCC -> APB1ENR |= RCC_APB1ENR_UART4EN;
	}
	if(Config.Port == UART5)
	{
		fck = SystemCoreClock/2;
		if(Config.low_power) return static_cast<int8_t>(error_codes::Low_power_for_UART_4n5);
		RCC -> APB1ENR |= RCC_APB1ENR_UART5EN;
	}
	if(Config.Port == USART6)
	{
		fck = SystemCoreClock;
		if(Config.low_power) RCC -> APB2LPENR |= RCC_APB2LPENR_USART6LPEN;
		else RCC -> APB2ENR |= RCC_APB2ENR_USART6EN;
	}
	else
	{
		return static_cast<int8_t>(error_codes::Peripheral_Clock);
	}

	if(Config.data_word_length == static_cast<uint8_t>(Data_Word_Length::Bit8))
		Config.Port -> CR1 &= ~USART_CR1_M;
	else
		Config.Port -> CR1 |=  USART_CR1_M;

	switch (Config.stop_bits)
	{
		case static_cast<uint8_t>(Stop_Bits::Bit0_5):
			Config.Port -> CR2 &= ~USART_CR2_STOP;
			break;
		case static_cast<uint8_t>(Stop_Bits::Bit1):
		{
			Config.Port -> CR2 |=  USART_CR2_STOP_0;
			Config.Port -> CR2 &=  ~USART_CR2_STOP_1;
		}
			break;
		case static_cast<uint8_t>(Stop_Bits::Bit1_5):
		{
			Config.Port -> CR2 &= ~USART_CR2_STOP_0;
			Config.Port -> CR2 |=  USART_CR2_STOP_1;
		}
			break;
		case static_cast<uint8_t>(Stop_Bits::Bit2):
			Config.Port -> CR2 |=  USART_CR2_STOP;
			break;
	}

	if((Config.parity == static_cast<uint8_t>(Parity::Enable)))
	{
		Config.Port -> CR1 |= USART_CR1_PCE;
		if(Config.parity == static_cast<uint8_t>(Parity::Even_Parity))
			Config.Port -> CR1 &= ~USART_CR1_PS;
		if(Config.parity == static_cast<uint8_t>(Parity::Odd_Parity))
			Config.Port -> CR1 |= USART_CR1_PS;
	}
	if((Config.parity == static_cast<uint8_t>(Parity::Disable)))
	{
		Config.Port -> CR1 &= ~USART_CR1_PCE;
	}

	switch (Config.interrupt)
	{
		case static_cast<uint8_t>(Interrupt::Disable):
		{
			Config.Port -> CR1 &= ~(USART_CR1_IDLEIE | USART_CR1_PEIE |
					                USART_CR1_TCIE   | USART_CR1_TXEIE |
									USART_CR1_RXNEIE );
		}
			break;

		case static_cast<uint8_t>(Interrupt::Idle):
		{
			Config.Port -> CR1 |= (USART_CR1_IDLEIE);
		}
			break;

		case static_cast<uint8_t>(Interrupt::Parity_Error):
		{
			Config.Port -> CR1 |= (USART_CR1_PEIE);
		}
			break;

		case static_cast<uint8_t>(Interrupt::Receiver_Not_Empty):
		{
			Config.Port -> CR1 |= (USART_CR1_RXNEIE);
		}
			break;

		case static_cast<uint8_t>(Interrupt::Transmit_Data_Register_Empty):
		{
			Config.Port -> CR1 |= (USART_CR1_TXEIE);
		}
			break;

		case static_cast<uint8_t>(Interrupt::Transmiter_Complete):
		{
			Config.Port -> CR1 |= (USART_CR1_TCIE);
		}
			break;
	}


	if(Config.dma_enable)
	{

	}
	else
	{
		Config.Port -> CR2 |= USART_CR3_DMAR;
	}


	float udiv = fck/(8 * 2 * (float)Config.baudrate);
    int16_t mantissa = (int32_t)udiv;
    int16_t frac = (int32_t)((udiv - (float)mantissa)*16);

    Config.Port -> BRR = (mantissa << 4) | (frac << 0);

    Config.Port -> CR1 |= USART_CR1_TE | USART_CR1_RE;
    Config.Port -> CR1 |= USART_CR1_UE;

    switch (Config.dma_setup)
    {
			case (DMA_Setup::Disable_DMA):
		{
				Config.Port -> CR3 &= ~(USART_CR3_DMAR | USART_CR3_DMAT);
		}
				break;
			case (DMA_Setup::Transmit_Only):
		{
				Config.Port -> CR3 |= (USART_CR3_DMAT);
				Config.USART_DMA.Init();
		}
				break;
			case (DMA_Setup::Receive_Only):
		{
				Config.Port -> CR3 |= (USART_CR3_DMAR);
				Config.USART_DMA.Init();
		}
				break;
			case (DMA_Setup::Both_Transmit_Receive):
		{
				Config.Port -> CR3 |= (USART_CR3_DMAT | USART_CR3_DMAR);
				Config.USART_DMA.Init();
		}
				break;
	}








	return static_cast<int8_t>(error_codes::ALL_OK);
}




void USART::Transmit_Byte(int data)
{
	if(Config.Port -> SR & USART_SR_TXE)
	{
		Config.Port -> DR = data;
	}
	while(!(Config.Port -> SR & USART_SR_TC));
}


int USART::Receive_Byte(void)
{
	uint8_t temp = 0;
	while((Config.Port -> SR & USART_SR_RXNE))
	{
		temp = Config.Port -> DR;
	}
	return temp;
}

void USART::Transmit_Buffer(void *transmit_buffer_address, uint16_t buffer_length)
{

}

void USART::Receive_Buffer(void *receive_buffer_address, uint16_t buffer_length)
{

}

