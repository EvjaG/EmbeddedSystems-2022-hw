#include <stdarg.h>
#include <stdio.h>
#include "stm32f303xe.h"
#include "usart2.h"



// This buffer is used by the printf-like print function.
static char A_Buffer[USART2_SIZE_OF_PRINT_BUFFER];




void print(char *p_format, ...)
{
	va_list p_variables;
	va_start(p_variables, p_format);
	(void)vsprintf(A_Buffer, p_format, p_variables);
    USART2_print(A_Buffer);
}




void USART2_init(void)
{
    // Enable GPIOA clock (p. 148 in the datasheet).
    RCC->AHBENR |= 0x00020000;
    // Enable USART2 clock.
    RCC->APB1ENR |= 0x00020000;
    // Configure GPIOA pins 2 and 3 as alternate function 7, which is USART2.
    GPIOA->MODER |= 0x000000A0;
    GPIOA->AFR[0] |= 0x00007700;
    USART2->BRR = 833;  // 8 MHz / 9600 baud rate.
    // Enable USART2, TX functionality, RX functionality, RXNE interrupt.
    USART2->CR2 = 0x00000000;//STOP Bit = 1 bit
    USART2->CR1 |= USART_CR1_UE | USART_CR1_TE;


}




void USART2_printCharacter(char c)
{
    USART2->TDR = c;
    while(!(USART2->ISR & 0x00000080));
}




void USART2_print(const char *p_data)
{
	while(*p_data != '\0')
	{
		USART2->TDR = *p_data;
        p_data++;
        while(!(USART2->ISR & 0x00000080));
	}
}
