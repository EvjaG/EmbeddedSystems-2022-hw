/***************************/
/* ex1 main file                                                          */
/* All references to the datasheet are to STM's RM0316, revision 8           */
/***************************/
#include <stdio.h>
#include <stdlib.h>
#include "stm32f303xe.h"
#include "usart2.h"
#include "spi1.h"


char timeFromSlave[9] = "";
static volatile uint8_t X;



/*
 * 8mhz= 8*(10**6)/sec
 * 4*10**6 = time = 3D0900
 *
 * */
char format[100] = "-time HH:MM:SS\nWhere:\n\t0<=HH<=24\n\t0<=MM<60\n\t0<=SS<60";

//this will be the button interrupt function
char* returnHour(){
	char timeFromSlave[8];
    SPI_Receive(timeFromSlave,8);
	return timeFromSlave;
}
//this will be the function from reading the input from the user
//return 0 if fail, otherwise return 1
int inputTime(char* input){
	SPI_Transmit(input,8);
	return 1;
}


//// ------------------------------------------------------ Button Handler function
//void EXTI15_10_IRQHandler(){
//	EXTI->PR |= 0x00002000;
//	// TODO - function to retrieve time from secondary machine
//	char* toPrint = returnHour();
//	print("%s\n",toPrint);
//	free(toPrint);
//}

//--------------------------------------------------------UART input handler function
void USART2_EXTI26_IRQHandler(void){

	if(RX_BUF_PLACE >= RX_BUF_SIZE)
		RX_BUF_PLACE=0;
	char newChar = (uint8_t)USART2->RDR;
	if(newChar==(uint8_t)'\0' || newChar==(uint8_t)'\n' || newChar==(uint8_t)'\r'){
		if (inputTime(RX_BUF)!=1)
			print("To change time please input in the following format  it:\t%s\n",format);
		RX_BUF_PLACE=0;
		memset(RX_BUF,'\0',RX_BUF_SIZE);
		return;
	}
	RX_BUF[RX_BUF_PLACE++]=newChar;

}


//--------------------------------------------------------SPI1 interrupt handler function
void SPI1_IRQHandler(void){
	memset(timeFromSlave,'\0',9);
	SPI_Receive(timeFromSlave,8);
}


// ------------------------------------------------------ Main
int main(void)
{
    // Enable GPIOA clock (p. 148 in the datasheet)
    RCC->AHBENR |=0x000E0000;
    RCC->APB1ENR |=  0x00000001; // enable TMR2
    RCC->APB2ENR|=  0x00000001; // enable SYSCFG Clock
// ------------------------------------------------------
    TIM2->DIER |= 0x00000001; //TIM2 interrupt enable
    EXTI->FTSR |= 0x00002000; // config falling edge GPIOC13
    EXTI->IMR |= 0x00002000; // enable interrupt GPIOC13
// ------------------------------------------------------
    SYSCFG->EXTICR[3] |= 0x00000020;
//    NVIC_EnableIRQ(EXTI15_10_IRQn); // enable button - core interrupt
// ------------------------------------------------------
    // Configure GPIOA pin 5 as output, 0 as input.
    GPIOA->MODER |= 0x00000400;
    // Configure GPIOA pin 0 as input.
//    GPIOA->MODER &= ~0x00000003;
    // Configure GPIOA pin 5 as push pull.
    GPIOA->OTYPER &= ~0x00000021; // (1 << 5);
//    GPIOA->ODR &= ~0x1;

    USART2_init();
	SPI1_init();
    NVIC_EnableIRQ(USART2_IRQn); //usart2 rx interrupt function enable
    print("Hello!\nThis is the primary machine in the 2-machine exercise you are running!\n");
    print("To change time please input in the following format  it:\t%s\n",format);
	memset(timeFromSlave,'\0',9);

	while(1){
		if((GPIOA->IDR&0x100)==0x100){
			SPI_Receive(timeFromSlave, 8);
			print("We just detect a movement at %s\n",timeFromSlave);
			memset(timeFromSlave,'\0',9);

		}
		else{
			int i = 1;
		}
	}

}
