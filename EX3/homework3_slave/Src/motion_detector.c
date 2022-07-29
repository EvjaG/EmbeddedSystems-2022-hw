#include "stm32f303xe.h"
#include "usart2.h"

void MotDet_init(void)
{
//    // Enable GPIOA clock (p. 148 in the datasheet).
//    RCC->AHBENR |= 0x00020000;
//    // Enable USART2 clock.
//    RCC->APB1ENR |= 0x00020000;
//    // Configure GPIOA pins 2 and 3 as alternate function 7, which is USART2.
//    GPIOA->MODER |= 0x000000A0;
//    GPIOA->AFR[0] |= 0x00007700;
//    USART2->BRR = 833;  // 8 MHz / 9600 baud rate.
//    USART2->CR2 = 0x00000000;//STOP Bit = 1 bit
//    USART2->CR1 |= USART_CR1_UE | USART_CR1_TE;
}
