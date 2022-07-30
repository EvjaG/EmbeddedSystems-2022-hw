/*
 * usart2.h
 *
 *  Created on: Mar 17, 2021
 *      Author: elien
 */

#define RX_BUF_SIZE 20
extern char RX_BUF[RX_BUF_SIZE];
extern int	RX_BUF_PLACE;
#ifndef USART2_H_
#define USART2_H_
#include <string.h>
/******************************************************************************
Definitions
******************************************************************************/
#define USART2_SIZE_OF_PRINT_BUFFER 128




/******************************************************************************
Exported functions
******************************************************************************/

/******************************************************************************
print
Description: Functions as the stdio printf function.
Note:
  This function uses a buffer, whose size is determined by USART2_SIZE_OF_
  PRINT_BUFFER. Therefore, it is not reentrant.
******************************************************************************/
void print(char *p_format, ...);

/******************************************************************************
USART2_init
Description: Initializes USART2 on GPIOs PA2 and PA3,
  with a baud rate of 9600.
******************************************************************************/
void USART2_init(void);

/******************************************************************************
USART2_print
Description: Prints a '\0'-terminated sequence of characters.
Input:
  p_data: The sequence to print.
******************************************************************************/
void USART2_print(const char *p_data);

/******************************************************************************
USART2_printCharacter
Description: Prints one character.
Input:
  c: The character to print.
******************************************************************************/
void USART2_printCharacter(char c);

#endif /* USART2_H_ */
