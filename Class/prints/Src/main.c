/*****************************************************************************/
/* Prints main file                                                           */
/* All references to the datasheet are to STM's RM0316, revision 8           */
/*****************************************************************************/




#include "stm32f303xe.h"
#include "usart2.h"




int main(void)
{
	USART2_init();
	USART2_printCharacter('t');
	USART2_printCharacter('\n');
    USART2_print("I love pizza!\n");
	print("Give me %d slices of that pizza!\n", 10);

    while(1)
    {
    }
}
