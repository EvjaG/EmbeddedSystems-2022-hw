#include <stdarg.h>
#include <stdio.h>
#include "stm32f303xe.h"
#include "USART3.h"

char RX_BUF3[RX_BUF3_SIZE];
int	RX_BUF3_PLACE;

// This buffer is used by the printf-like print function.
static char A_Buffer[USART3_SIZE_OF_PRINT_BUFFER];




void print3(char *p_format, ...)
{
	va_list p_variables;
	va_start(p_variables, p_format);
	(void)vsprintf(A_Buffer, p_format, p_variables);
    USART3_print(A_Buffer);
}




void USART3_init(void)
{
    // Enable GPIOA clock (p. 148 in the datasheet).
    RCC->AHBENR |= 0x00020000;
    // Enable USART3 clock.
    RCC->APB1ENR |= 0x00040000;
    // Configure GPIOB pins 8 and 9 as alternate function 7, which is USART3.
    GPIOB->MODER |= 0x000A0000;
    GPIOA->AFR[0] |= 0x00000077;
    USART3->BRR = 833;  // 8 MHz / 9600 baud rate.
    // Enable USART3, TX functionality, RX functionality, RXNE interrupt.
    USART3->CR2 = 0x00000000;//STOP Bit = 1 bit
    memset(RX_BUF3,'\0',RX_BUF3_SIZE);
    RX_BUF3_PLACE=0;
    USART3->CR1 |= USART_CR1_RXNEIE | USART_CR1_RE | USART_CR1_UE | USART_CR1_TE;


}




void USART3_printCharacter(char c)
{
    USART3->TDR = c;
    while(!(USART3->ISR & 0x00000080));
}




void USART3_print(const char *p_data)
{
	while(*p_data != '\0')
	{
		USART3->TDR = *p_data;
        p_data++;
        while(!(USART3->ISR & 0x00000080));
	}
}
