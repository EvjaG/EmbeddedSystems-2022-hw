#include <spi1.h>
#include <stdarg.h>
#include <stdio.h>
#include "stm32f303xe.h"



// This buffer is used by the printf-like print function.

int pointer = 0;
int A_Buffer_PLACE=0;

void SPI1_send(const char *p_data)
{
//	while(*p_data != '\0')
//	{
//		USART2->TDR = *p_data;
//        p_data++;
//        while(!(USART2->ISR & 0x00000080));
//	}
	//todo
	return;
}

void send(char *p_format, ...)
{
	va_list p_variables;
	va_start(p_variables, p_format);
	(void)vsprintf(SPI_A_Buffer, p_format, p_variables);
    SPI1_send(SPI_A_Buffer);
}




void SPI1_init(void)
{
	//TODO - initialize SPI values (page 962 of user manual)
	memset(SPI_A_Buffer,'\0',SPI1_SIZE_OF_SPI_BUFFER);
	memset(SPI_B_Buffer,'\0',SPI1_SIZE_OF_SPI_BUFFER);
	*SPI_B_Buffer='\0';

	//APB1 - page 152
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;

	//CR1 - page 998
	SPI1->CR1 |= ~SPI_CR1_MSTR | SPI_CR1_CPHA | SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0; // configure as SECONDARY, CPHA on, BR = 111
	SPI1->CR1 |= ~SPI_CR1_BIDIMODE;

	//CR2 - page 1000
	SPI1->CR2 |= SPI_CR2_TXEIE| SPI_CR2_RXNEIE | SPI_CR2_DS_0 |SPI_CR2_DS_1 | SPI_CR2_DS_2 | SPI_CR2_FRXTH;
}



