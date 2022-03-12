/*********/
/* Blinky main file                                                          */
/* All references to the datasheet are to STM's RM0316, revision 8           */
/*********/




#include "stm32f303xe.h"
/*
 * 8mhz= 8*(10**6)/sec
 * 4*10**6 = time = 3D0900
 *
 * */


//int i=0;
int main(void)
{

    // Enable GPIOA clock (p. 148 in the datasheet)
    RCC->AHBENR |= 0x00020000;
    RCC->APB1ENR |=  0x00000001;
    // Configure GPIOA pin 5 as output.
    GPIOA->MODER |= 0x00000400;
    // Configure GPIOA pin 5 as push pull.
    GPIOA->OTYPER &= ~0x00000020; // (1 << 5);
//    wait1 = 0x003D0900;
    TIM2->ARR=0x003D0900; // arr = 4 million
    TIM2->CR1|=0x00000001;
    int counter = 0;
    while(1)
    {
    	// Toggle GPIOA pin 5.
    	if((TIM2->SR&0x00000001)==0x00000001){
    		TIM2->SR&=0XFFFFFFFE;
    		GPIOA->ODR ^= 0x00000020; // Write 0x00000020 to the address 0x48000014
    		if(counter == 0 && TIM2->ARR > 0x3D09){
    			TIM2->ARR/=2;
    		}
    		counter ^= 1;

    	}

        // Use an empty for loop to delay.
//        for(i = 0; i < 100000; i++);
    }
}
