#include <spi1.h>
#include <stdarg.h>
#include <stdio.h>
#include "stm32f303xe.h"



// This buffer is used by the printf-like print function.

int pointer = 0;
int A_Buffer_PLACE=0;

//void SPI1_send(const char *p_data)
//{
////	while(*p_data != '\0')
////	{
////		USART2->TDR = *p_data;
////        p_data++;
////        while(!(USART2->ISR & 0x00000080));
////	}
//	//todo
//	return;
//}
//
//void send(char *p_format, ...)
//{
//	va_list p_variables;
//	va_start(p_variables, p_format);
//	(void)vsprintf(SPI_A_Buffer, p_format, p_variables);
//    SPI1_send(SPI_A_Buffer);
//}
//
//


void SPI1_init(void)
{
	//TODO - initialize SPI values (page 962 of user manual)
	memset(SPI_A_Buffer,'\0',SPI1_SIZE_OF_SPI_BUFFER);
	memset(SPI_B_Buffer,'\0',SPI1_SIZE_OF_SPI_BUFFER);
	*SPI_B_Buffer='\0';

//	RCC->AHBENR |= (1<<0);  // Enable GPIO Clock

	//APB1 - page 152
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;// Enable SPI1 CLock

	//CR1 - page 998
//	SPI1->CR1 |= SPI_CR1_CPHA | SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0; // CPHA on, BR = 111
//	SPI1->CR1 &= ~SPI_CR1_BIDIMODE & ~SPI_CR1_MSTR; //not Bidirectional data mode enable //Slave Mode
//
//
//	SPI1->CR1 &= ~(1<<11);  // DFF=0, 8 bit data
//	SPI1->CR1 |= (1<<6);   // SPE=1, Peripheral enabled
//
//	SPI1->CR2 = 0;  // reset CR2
//	//CR2 - page 1000
//	//SPI_CR2_TXEIE -> Tx buffer Empty Interrupt Enable
//	//SPI_CR2_RXNEIE -> RX buffer Not Empty Interrupt Enable
//	//SPI_CR2_DS_0|SPI_CR2_DS_1|SPI_CR2_DS_2 -> 0111: 8-bit Data size
//	//SPI_CR2_FRXTH -> FIFO reception Threshold
//	SPI1->CR2 |= SPI_CR2_TXEIE| SPI_CR2_RXNEIE | SPI_CR2_DS_0 |SPI_CR2_DS_1 | SPI_CR2_DS_2 | SPI_CR2_FRXTH;
	SPI1->CR2 |= 0x000000040;
	SPI1->CR1 |= 0x000000040;


	GPIOA->MODER |= 0x0000A200; // Configure GPIOA pins 4,6,7  as alternate function 5, which is SPI1.
	GPIOA->AFR[0] |= (5<<16)|(5<<24)|(5<<28);// AF5(SPI1) for PA4, PA6, PA7
	GPIOB->MODER |= 0x00000020; // Configure GPIOA pins 4,6,7  as alternate function 5, which is SPI1.
	GPIOB->AFR[0] |= (5<<12);// AF5(SPI1) for PA4, PA6, PA7

	GPIOA->MODER &= 0xFFFFAAFF;
	GPIOA->MODER |= 0xAA00;



}


void SPI_Transmit (char *data, int size)
{




	int i=0;
	while (i<size)
	{
	   SPI1->DR = data[i];  // load the data into the Data Register
//	   while (!((SPI1->SR)&(1<<1))) {};  // wait for TXE bit to set -> This will indicate that the buffer is empty
//	   SPI1->DR = data[i];  // load the data into the Data Register
	   uint8_t temp = SPI1->DR;
	   i++;
	}



//	/************** STEPS TO FOLLOW *****************
//	1. Wait for the TXE bit to set in the Status Register
//	2. Write the data to the Data Register
//	3. After the data has been transmitted, wait for the BSY bit to reset in Status Register
//	4. Clear the Overrun flag by reading DR and SR
//	************************************************/
//
//	int i=0;
//	while (i<size)
//	{
//	   while (!((SPI1->SR)&(1<<1))) {};  // wait for TXE bit to set -> This will indicate that the buffer is empty
//	   SPI1->DR = data[i];  // load the data into the Data Register
//	   i++;
//	}
//
//
///*During discontinuous communications, there is a 2 APB clock period delay between the
//write operation to the SPI_DR register and BSY bit setting. As a consequence it is
//mandatory to wait first until TXE is set and then until BSY is cleared after writing the last
//data.
//*/
//	while (!((SPI1->SR)&(1<<1))) {};  // wait for TXE bit to set -> This will indicate that the buffer is empty
//	while (((SPI1->SR)&(1<<7))) {};  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication
//
//	//  Clear the Overrun flag by reading DR and SR
//	uint8_t temp = SPI1->DR;
//	temp = SPI1->SR;

}


void SPI_Receive (char *data, int size)
{
	/************** STEPS TO FOLLOW *****************
	1. Wait for the BSY bit to reset in Status Register
	2. Send some Dummy data before reading the DATA
	3. Wait for the RXNE bit to Set in the status Register
	4. Read data from Data Register
	************************************************/

	while (size)
	{
		while (((SPI1->SR)&(1<<7))) {};  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication
		SPI1->DR = 0;  // send dummy data
		while (!((SPI1->SR) &(1<<0))){};  // Wait for RXNE to set -> This will indicate that the Rx buffer is not empty
	  *data++ = (SPI1->DR);
		size--;
	}
}



