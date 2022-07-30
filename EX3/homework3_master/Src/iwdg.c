#include <stdarg.h>
#include <stdio.h>
#include "stm32f303xe.h"
#include "iwdg.h"

void IWDG_init(void){
	IWDG->KR = 0xCCCC; // Enable IWDG
	IWDG->KR = 0x5555; // Enable IWDG register access
	IWDG->PR = 7;	   // Maximum value for pre-scaler
	IWDG->RLR = 0xFFF; //Maximum value for counter
	// formula:
	// TLSI = 1/40khz=2.5E-5
	// PR, RLR = user-set
	// timeout = TLSI * 2 * ( 2 ^ PR ) * (RLR+1)
	// with the given values, the timeout is set to 26.21 seconds!

	while(IWDG->SR != 0){} //wait for SR to reach 0
	IWDG->KR = 0xAAAA; // Refresh counter

}
void IWDG_refresh(void){
	IWDG->KR = 0xAAAA; // Refresh counter
}
