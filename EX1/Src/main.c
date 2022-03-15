/***************************/
/* Blinky main file                                                          */
/* All references to the datasheet are to STM's RM0316, revision 8           */
/***************************/
#include "stm32f303xe.h"

/*
 * 8mhz= 8*(10**6)/sec
 * 4*10**6 = time = 3D0900
 *
 * */



// ------------------------------------------------------initializing global variables
typedef enum
{
    on=2,
    off=1,
    blink=0
} currentMode;

int mode = 0;



// ------------------------------------------------------ Button Handler function
void EXTI15_10_IRQHandler(){// 3 modes : x%3 = 0 :off x%3=1 : blinking x%3 = 2 : on
	EXTI->PR |= 0x00002000;
	mode++;
	mode%=3;
	currentMode curr = mode;
	NVIC_DisableIRQ(TIM2_IRQn);// enable interrupt core
	switch(curr)
	{
		case on:
			GPIOA->ODR |= ~0xFFFFFFDF;
			break;

		case blink:
			NVIC_EnableIRQ(TIM2_IRQn);// enable interrupt core
			break;

		case off:
			GPIOA->ODR &= 0xFFFFFFDF;
	}
}


// ------------------------------------------------------Timer handler function
void TIM2_IRQHandler(){
	TIM2->SR&=0XFFFFFFFE;
	GPIOA->ODR ^= 0x00000020; // Write 0x00000020 to the address 0x48000014
}



// ------------------------------------------------------ Main
int main(void)
{
    // Enable GPIOA clock (p. 148 in the datasheet)
    RCC->AHBENR |=0x000A0000;
    RCC->APB1ENR |=  0x00000001; // enable TMR2
    RCC->APB2ENR|=  0x00000001; // enable SYSCFG Clock
// ------------------------------------------------------
    TIM2->DIER |= 0x00000001; //interrupt enable
    EXTI->FTSR |= 0x00002000; // config falling edge GPIOC13
    EXTI->IMR |= 0x00002000; // enable interrupt GPIOC13
// ------------------------------------------------------
    SYSCFG->EXTICR[3] |= 0x00000020;
    NVIC_EnableIRQ(EXTI15_10_IRQn); // enable button - core interrupt
// ------------------------------------------------------
    // Configure GPIOA pin 5 as output.
    GPIOA->MODER |= 0x00000400;
    // Configure GPIOA pin 5 as push pull.
    GPIOA->OTYPER &= ~0x00000020; // (1 << 5);

    TIM2->ARR= 4000000; // same as writing TIM2->ARR =0x003D0900
    TIM2->CR1|=0x00000001;


    while(1)
    {

    }

}
