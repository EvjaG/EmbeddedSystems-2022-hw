/***************************/
/* ex1 main file                                                          */
/* All references to the datasheet are to STM's RM0316, revision 8           */
/***************************/
#include <stdio.h>
#include <stdlib.h>
#include "stm32f303xe.h"
#include "usart2.h"
#include "spi1.h"

/*
 * 8mhz= 8*(10**6)/sec
 * 4*10**6 = time = 3D0900
 *
 * */

int second=0;
int minute=0;
int hour=0;
int flip=1;
int first=0;
int motdet = 1;

char* str = "1234567";

//this will be the button interrupt function
char* returnHour(){
    char* toReturn = (char*) malloc(10*sizeof(char));
	sprintf(toReturn,"%02d:%02d:%02d",hour,minute,second);
	return toReturn;
}
// power function
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


	//check string inputs
	char *token = strtok(input, " "); // doin manipulation over the string we recieved. (swaping all the panctual to " ")
	token = strtok(NULL, ":");

	int j=0;
	int modArr[3]={24,60,60}; // we put each digit in the right place in our array and perfrmoing MOD action to make sure the hour is in bounds
	int timeArr[3]={0,0,0};

	while (token != NULL)
	{
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
	return 1;
}




void send_clock(){
//	GPIOA->ODR |= 0x00000100; // Write 0x00000001 to A0
//	GPIOA->ODR &= ~0x00000100; // Write 0 to A0
	GPIOA->ODR ^= 0x00000100;
	print("in send_clock time is = %s\n",returnHour());
	SPI_Transmit(returnHour(), 8);
}


// ------------------------------------------------------ Button Handler function
void EXTI15_10_IRQHandler(){ // turning off the interrupt flag
	EXTI->PR |= 0x00002000;
	char* toPrint = returnHour(); // printing the current time
//	SPI_Transmit(str,8);
	print("%s\n",toPrint);
	free(toPrint);
}


// ------------------------------------------------------Timer handler function
void TIM2_IRQHandler(void){
	flip^=1; // for full-second check
	if(first && flip){
		//if we've been to the function at least once
		increaseSec(); // increase time by 1 second
		motdet=1;
	}
	first = 1; //indicate we've visited function at least once
	GPIOA->ODR ^= 0x00000020; // Write 0x00000020 to the address 0x48000014


	TIM2->SR&=0XFFFFFFFE; // reenable timer interrupt


}


//------------------------------------------------------ Main
int main(void)
{
    // Enable GPIOA clock (p. 148 in the datasheet)
    RCC->AHBENR |=0x000E0000;
    RCC->APB1ENR |=  0x00000001; // enable TMR2
    RCC->APB2ENR|=  0x00000001; // enable SYSCFG Clock
// ------------------------------------------------------
    TIM2->DIER |= 0x00000001; //TIM2 interrupt enable
    EXTI->FTSR |= 0x00002000; // config falling edge GPIOC13
    EXTI->IMR |= 0x00002000; // enable interrupt GPIOC13
// ------------------------------------------------------
    SYSCFG->EXTICR[3] |= 0x00000020;
// ------------------------------------------------------
    // Configure GPIOA pin 5,0 as output .
    GPIOA->MODER |= 0x00010400;
    // Configure GPIOA pin 5 as push pull.
    GPIOA->OTYPER &= ~0x00000121; // (1 << 5);





    TIM2->ARR= 8000000/2; // same as writing TIM2->ARR =0x003D0900*2 = the timer2 interrupt speed
    TIM2->CR1|=0x00000001; // TIM2 counter enable
    NVIC_EnableIRQ(TIM2_IRQn); //TIM2 interrupt function enable


    GPIOA->ODR=0;

    USART2_init();
    SPI1_init();
    print("Hello!\nThis is the secondary machine in the 2-machine exercise you are running!\n");

    while(1)
    {
    	if((GPIOA->IDR&0x2)==0x2){
    		if(motdet){
				print("MotDet ON!");
				send_clock();
				motdet=0;
    		}
    	}
    	if((SPI1->SR&0x3)==0x3){
    		memset(SPI_A_Buffer,'\0',9);
//    		print("going to spi recive");
    		char * copySPI_A_Buffer = SPI_A_Buffer;
    		SPI_Receive(SPI_A_Buffer,8);
//    		print("return from spi recive");
    		if(copySPI_A_Buffer[0] != '\0'){
    			print("time recived = %s\n",copySPI_A_Buffer);
    			inputTime(copySPI_A_Buffer);
    		}
    	}

    }

}
