#include <spi1.h>
#include <stdarg.h>
#include <stdio.h>
#include "stm32f303xe.h"




int pointer = 0;
int A_Buffer_PLACE=0;



void SPI1_init(void)
{
	//TODO - initialize SPI values (page 962 of user manual)
	memset(SPI_A_Buffer,'\0',SPI1_SIZE_OF_SPI_BUFFER);
	memset(SPI_B_Buffer,'\0',SPI1_SIZE_OF_SPI_BUFFER);
	*SPI_B_Buffer='\0';

	//APB1 - page 152
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;// Enable SPI1 CLock

	GPIOA->MODER |= 0x0000A200; // Configure GPIOA pins 4,6,7  as alternate function 5, which is SPI1.
	GPIOA->AFR[0] |= (5<<16)|(5<<24)|(5<<28);// AF5(SPI1) for PA4, PA6, PA7
	GPIOB->MODER |= 0x00000080; // Configure GPIOB pins 3  as alternate function 5, which is SPI1.
	GPIOB->AFR[0] |= (5<<12);// AF5(SPI1) for PB3
	SPI1->CR2 |= 0x000000044;
	SPI1->CR1 |= 0x00000007C;

}


void SPI_Transmit (char *data, int size)
{

	int i=0;
	while (i<size){
	   SPI1->DR = (uint8_t)data[i];  // load the data into the Data Register
	   print("in SPI_Transmit data[%d] = %c\n",i,data[i]);
	   while((SPI1->SR&0x3)!=0x3 && (SPI1->SR&0x80)!=0x80){}
	   uint8_t temp = SPI1->DR;
	   i++;
	}

}

void SPI_Receive (char *data, int size)
{
//	print("in SPI_Receive\n");
	while (size){
//		print("in SPI_Receive size = %d\n",size);
		SPI1->DR = 0;  // send dummy data
		while((SPI1->SR&0x3)!=0x3){}
		*data++ = (SPI1->DR);
		size--;
	}
//	print("in SPI_Receive data recived = %s\n",data);

}



