/*
 * USART3.h
 *
 *  Created on: Mar 17, 2021
 *      Author: elien
 */

#define RX_BUF3_SIZE 20
extern char RX_BUF3[RX_BUF3_SIZE];
extern int	RX_BUF3_PLACE;
#ifndef USART3_H_
#define USART3_H_
#include <string.h>
/******************************************************************************
Definitions
******************************************************************************/
#define USART3_SIZE_OF_PRINT_BUFFER 128




/******************************************************************************
Exported functions
******************************************************************************/

/******************************************************************************
print
Description: Functions as the stdio printf function.
Note:
  This function uses a buffer, whose size is determined by USART3_SIZE_OF_
  PRINT_BUFFER. Therefore, it is not reentrant.
******************************************************************************/
void print3(char *p_format, ...);

/******************************************************************************
USART3_init
Description: Initializes USART3 on GPIOs PA2 and PA3,
  with a baud rate of 9600.
******************************************************************************/
void USART3_init(void);

/******************************************************************************
USART3_print
Description: Prints a '\0'-terminated sequence of characters.
Input:
  p_data: The sequence to print.
******************************************************************************/
void USART3_print(const char *p_data);

/******************************************************************************
USART3_printCharacter
Description: Prints one character.
Input:
  c: The character to print.
******************************************************************************/
void USART3_printCharacter(char c);

#endif /* USART3_H_ */
