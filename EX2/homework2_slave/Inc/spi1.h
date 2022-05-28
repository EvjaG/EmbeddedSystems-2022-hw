/*
 * spi2.h
 *
 *  Created on: Mar 17, 2021
 *      Author: elien
 */


#ifndef SPI1_H_
#define SPI1_H_
#include <string.h>
/******************************************************************************
Definitions
******************************************************************************/
#define SPI1_SIZE_OF_SPI_BUFFER 128
static char SPI_A_Buffer[SPI1_SIZE_OF_SPI_BUFFER];
static char SPI_B_Buffer[SPI1_SIZE_OF_SPI_BUFFER];



/******************************************************************************
Exported functions
******************************************************************************/

/******************************************************************************
print
Description: Functions as the stdio printf function.
Note:
  This function uses a buffer, whose size is determined by spi2_SIZE_OF_
  PRINT_BUFFER. Therefore, it is not reentrant.
******************************************************************************/
void send(char *p_format, ...);

/******************************************************************************
spi2_init
Description: Initializes spi2 on GPIOs PA2 and PA3,
  with a baud rate of 9600.
******************************************************************************/
void SPI1_init(void);

/******************************************************************************
spi2_print
Description: Prints a '\0'-terminated sequence of characters.
Input:
  p_data: The sequence to print.
******************************************************************************/
void SPI1_print(const char *p_data);

/******************************************************************************
spi2_printCharacter
Description: Prints one character.
Input:
  c: The character to print.
******************************************************************************/
void SPI1_printCharacter(char c);

/******************************************************************************
SPI1_send
Description: send all chars to master.
Input:
  c: The character to print.
******************************************************************************/
//void SPI1_send(const char *p_data);


void SPI_Transmit (char *data, int size);


void SPI_Receive (char *data, int size);

#endif /* SPI1_H_ */
