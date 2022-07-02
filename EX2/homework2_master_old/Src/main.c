/***************************/
/* ex1 main file                                                          */
/* All references to the datasheet are to STM's RM0316, revision 8           */
/***************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stm32f303xe.h"
#include "usart2.h"
#include "spi1.h"


char timeFromSlave[9] = "";
static volatile uint8_t X;
int kdet = 0,kdet_prev = 0;



/*
 * 8mhz= 8*(10**6)/sec
 * 4*10**6 = time = 3D0900
 *
 * */
char format[100] = "-time HH:MM:SS\nWhere:\n\t0<=HH<=24\n\t0<=MM<60\n\t0<=SS<60";

// power function
int power(int a,int b){
	int toReturn=1;
	for (int i=0;i<b;i++)
		toReturn*=a;
	return toReturn;
}

int validateTime(char* input){
	//parse string - check for correct format
	int len;
	int colon_count=0;
	int func_count=0;
	int space_count=0;
	for (len = 0; input[len] != '\0'; len++){ // were checking to see what punctual the user used in the hour he put inside
		if(input[len]==':') colon_count++;
		if(input[len]==' ') space_count++;
		if(input[len]=='-'){
			if(len>0 || func_count)
				return 0;
			func_count++;
		}

	}
	if(colon_count != 2 || func_count != 1 || input[0] != '-') return -1; // we check if the input stands in the format we want if not we return a msg.
	//if func call not in system
	if(strstr(input,"-time")==NULL)
		return -1;

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
	return 1;
}



//this will be the button interrupt function
char* returnHour(){
	char timeFromSlave[8];
    SPI_Receive(timeFromSlave,8);
	return timeFromSlave;
}
//this will be the function from reading the input from the user
//return 0 if fail, otherwise return 1
int inputTime(char* input){
//	print("in inputTime \n");
	if(validateTime(input) == 1){
//		print("in inputTime validation\n");
		SPI_Transmit(input+6,8);
//		print("in inputTime after trans\n");
		return 1;
	}
	else{
//		print("in inputTime validation failed\n");
		return -1;
	}
}


//// ------------------------------------------------------ Button Handler function
void EXTI15_10_IRQHandler(){
	EXTI->PR |= 0x00002000;
	// TODO - function to retrieve time from secondary machine
	char* toPrint = returnHour();
	print("%s\n",toPrint);
	free(toPrint);
}

//--------------------------------------------------------UART input handler function
void USART2_EXTI26_IRQHandler(void){

	if(RX_BUF_PLACE >= RX_BUF_SIZE)
		RX_BUF_PLACE=0;
	char newChar = (uint8_t)USART2->RDR;
	if(newChar==(uint8_t)'\0' || newChar==(uint8_t)'\n' || newChar==(uint8_t)'\r'){
		if (inputTime(RX_BUF)!=1)
			print("To change time please input in the following format  it:\t%s\n",format);
		RX_BUF_PLACE=0;
		memset(RX_BUF,'\0',RX_BUF_SIZE);
		return;
	}
	RX_BUF[RX_BUF_PLACE++]=newChar;

}


//--------------------------------------------------------SPI1 interrupt handler function
void SPI1_IRQHandler(void){
	memset(timeFromSlave,'\0',9);
	SPI_Receive(timeFromSlave,8);
}


// ------------------------------------------------------ Main
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
//    NVIC_EnableIRQ(EXTI15_10_IRQn); // enable button - core interrupt
// ------------------------------------------------------
    // Configure GPIOA pin 5 as output, 0 as input.
    GPIOA->MODER |= 0x00000400;
    // Configure GPIOA pin 0 as input.
//    GPIOA->MODER &= ~0x00000003;
    // Configure GPIOA pin 5 as push pull.
    GPIOA->OTYPER &= ~0x00000021; // (1 << 5);
//    GPIOA->ODR &= ~0x1;

    USART2_init();
	SPI1_init();
    NVIC_EnableIRQ(USART2_IRQn); //usart2 rx interrupt function enable
    print("Hello!\nThis is the primary machine in the 2-machine exercise you are running!\n");
    print("To change time please input in the following format  it:\t%s\n",format);
	memset(timeFromSlave,'\0',9);

	while(1){
		memset(timeFromSlave,'\0',9);
		if((GPIOA->IDR&0x100)==0x100){
			kdet = 1;
		}
		else{
			kdet = 0;
		}
		if(kdet_prev != kdet){
			SPI_Receive(timeFromSlave, 8);
			if(timeFromSlave[2] == ':'){
				print("We just detect a movement at %s\n",timeFromSlave);
				memset(timeFromSlave,'\0',9);
			}
			kdet_prev = kdet;
		}
	}

}
