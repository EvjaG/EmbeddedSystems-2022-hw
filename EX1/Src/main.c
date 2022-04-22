/***************************/
/* ex1 main file                                                          */
/* All references to the datasheet are to STM's RM0316, revision 8           */
/***************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "stm32f303xe.h"
#include "usart2.h"
/*
 * 8mhz= 8*(10**6)/sec
 * 4*10**6 = time = 3D0900
 *
 * */

int second=0;
int minute=0;
int hour=0;

//this will be the button interrupt function
char* returnHour(){
    char* toReturn = (char*) malloc(10*sizeof(char));
	sprintf(toReturn,"%02d:%02d:%02d",hour,minute,second);
	return toReturn;
}

int power(int a,int b){
	int toReturn=1;
	for (int i=0;i<b;i++)
		toReturn*=a;
	return toReturn;
}
//this will be the timer interrupt function
void increaseSec(){
	int minutePass=0;
	int hourPass=0;
	second++;
	if(second==60){
		second=0;
		minutePass=1;
	}
	if(minutePass){
		minute++;
		if(minute==60){
				minute=0;
				hourPass=1;
		}
	}
	if(hourPass){
			hour++;
			if(hour==24){
					hour=0;
			}
		}

}

//this will be the function from reading the input from the user
//return 0 if fail, otherwise return 1
int inputTime(char* input){
	//parse string - check for correct format
	int len;
	int colon_count=0;
	int func_count=0;
	int space_count=0;
	for (len = 0; input[len] != '\0'; len++){
		if(input[len]==':') colon_count++;
		if(input[len]==' ') space_count++;
		if(input[len]=='-'){
			if(len>0 || func_count)
				return 0;
			func_count++;
		}

	}
	if(colon_count != 2 || func_count != 1) return -1;
	//if func call not in system
	if(strstr(input,"-time")==NULL)
		return -1;

	//check string inputs
	char *token = strtok(input, " ");
	token = strtok(NULL, ":");

	int j=0;
	int modArr[3]={60,60,24};
	int timeArr[3]={0,0,0};

	while (token != NULL)
	{
//		printf("String:\t%s\n", token);
        int sum=0;
		for(int i=0;i<2;i++){
			int c = token[i];
			if(c<48 || c>57) //check if char is digit
				return 0;
			c-=48;
			sum+=c*power(10, 2-(i+1));
		}
		if(sum<0 || sum>=modArr[j])
			return 0;
//		printf("sum:\t%d\n",sum);
		timeArr[j]=sum;

		token = strtok(NULL, ":");
		j++;
	}
	//if no errors, change time vars accordingly
	hour=timeArr[0];
	minute=timeArr[1];
	second=timeArr[2];
	return 0;
}


// ------------------------------------------------------ Button Handler function
void EXTI15_10_IRQHandler(){
//	print("%d\n",second);
	EXTI->PR |= 0x00002000;
	char* toPrint = returnHour();
	print("***\nThe current time is:\t%s\n***\n",toPrint);
	free(toPrint);
//	mode++;
//	mode%=3;
//	currentMode curr = mode;
//	NVIC_DisableIRQ(TIM2_IRQn);// enable interrupt core
//	switch(curr)
//	{
//		case on:
//			GPIOA->ODR |= ~0xFFFFFFDF;
//			break;
//
//		case blink:
//			NVIC_EnableIRQ(TIM2_IRQn);// enable interrupt core
//			break;
//
//		case off:
//			GPIOA->ODR &= 0xFFFFFFDF;
//	}
}


// ------------------------------------------------------Timer handler function
void TIM2_IRQHandler(){
	increaseSec(); // increase time by 1 second
	TIM2->SR&=0XFFFFFFFE; // reenable timer interrupt
//	GPIOA->ODR ^= 0x00000020; // Write 0x00000020 to the address 0x48000014
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
    TIM2->ARR= 8000000; // same as writing TIM2->ARR =0x003D0900*2 = the timer2 interrupt speed
    TIM2->CR1|=0x00000001;
    NVIC_EnableIRQ(TIM2_IRQn);
    USART2_init();
    print("Hello!\n");
//    while(1)
//    {
//
//    }

}
