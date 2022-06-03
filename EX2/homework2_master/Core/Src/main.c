/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include "stm32f303xe.h"
#include "usart2.h"

#include "main.h"

char format[100] = "-time HH:MM:SS\nWhere:\n\t0<=HH<=24\n\t0<=MM<60\n\t0<=SS<60";
char spi_buf[20];

char USART2_buff [20];

int state = 3;


//this will be the function from reading the input from the user
//return 0 if fail, otherwise return 1
int inputUSART2(char* input){
	strcpy(USART2_buff,input);
	state = 0;
	return 1;
}


//--------------------------------------------------------UART input handler function
void USART2_EXTI26_IRQHandler(void){

	if(RX_BUF_PLACE >= RX_BUF_SIZE)
		RX_BUF_PLACE=0;
	char newChar = (uint8_t)USART2->RDR;
	if(newChar==(uint8_t)'\0' || newChar==(uint8_t)'\n' || newChar==(uint8_t)'\r'){
		if (inputUSART2(RX_BUF)!=1)
			print("To change time please input in the following format  it:\t%s\n",format);
		RX_BUF_PLACE=0;
		memset(RX_BUF,'\0',RX_BUF_SIZE);
		return;
	}
	RX_BUF[RX_BUF_PLACE++]=newChar;

}



// ------------------------------------------------------- SPI1 stuff end












// ------------------------------------------------------ Button Handler function
void EXTI15_10_IRQHandler(){
//	EXTI->PR |= 0x00002000;
//	// TODO - function to retrieve time from secondary machine
//	char* toPrint = returnHour();
//	print("%s\n",toPrint);
//	free(toPrint);
}
//--------------------------------------------------------UART input handler function
void HAL_USART_TxCpltCallback(void){

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



// ------------------------------------------------------Timer handler function
void TIM2_IRQHandler(void){
	GPIOA->ODR ^= 0x00000020; // Write 0x00000020 to the address 0x48000014
	TIM2->SR&=0XFFFFFFFE; // reenable timer interrupt
}





void HAL_SPI_TxCpltCallback (SPI_HandleTypeDef * hspi)
{
  // Set CS pin to high and raise flag
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
  spi_xmit_flag = 1;
}

// This is called when SPI receive is done
void HAL_SPI_RxCpltCallback (SPI_HandleTypeDef * hspi)
{
  // Set CS pin to high and raise flag
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
  spi_recv_flag = 1;
}


SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
// Enable GPIOA clock (p. 148 in the datasheet)
	RCC->AHBENR |=0x000A0000;
	RCC->APB1ENR |=  0x00000001; // enable TMR2
	RCC->APB2ENR|=  0x00000001; // enable SYSCFG Clock
// ------------------------------------------------------
	TIM2->DIER |= 0x00000001; //TIM2 interrupt enable
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
	TIM2->CR1|=0x00000001; // TIM2 counter enable
	NVIC_EnableIRQ(TIM2_IRQn); //TIM2 interrupt function enable

	USART2_init();


  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  NVIC_EnableIRQ(USART2_IRQn); //usart2 rx interrupt function enable
  print("Hello!\nThis is the primary machine in the 2-machine exercise you are running!\n");
  print("To change time please input in the following format  it:\t%s\n",format);
  char timeFromSlave[9];

  while(1)
  {
	  // Finite state machine to allow for non-blocking SPI transmit/receive
	      switch(state)
	      {
	        // Transmit
	        case 0:

	          // Fill buffer with stuff to write to EEPROM
	          for (int i = 0; i < 10; i++)
	          {
	            spi_buf[i] = USART2_buff[i];
	          }

	          // Enable write enable latch (allow write operations)
	          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
	          HAL_SPI_Transmit(&hspi1, (uint8_t *)&EEPROM_WREN, 1, 100);
	          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

	          // Perform non-blocking write to SPI
	          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
	          HAL_SPI_Transmit_IT(&hspi1, (uint8_t *)spi_buf, 12);

	          // Go to next state: waiting for interrupt flag
	          state += 1;

	          break;

	        // Wait for transmit flag
	        case 1:

	          if (spi_xmit_flag)
	          {
	            // Clear flag and go to next state
	            spi_xmit_flag = 0;
	            state += 1;
	          }

	          break;

	        // Wait for WIP bit to be cleared
	        case 2:

	          // Read status register
	          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
	          HAL_SPI_Transmit(&hspi1, (uint8_t *)&EEPROM_RDSR, 1, 100);
	          HAL_SPI_Receive(&hspi1, (uint8_t *)spi_buf, 1, 100);
	          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

	          // Mask out WIP bit
	          wip = spi_buf[0] & 0b00000001;

	          // If WIP is cleared, go to next state
	          if (wip == 0)
	          {
	            state += 1;
	          }

	          break;

	        // Set up for interrupt-based SPI receive
	        case 3:

	          // Clear SPI buffer
	          for (int i = 0; i < 12; i++)
	          {
	            spi_buf[i] = 0;
	          }

	          // Read the 10 bytes back
	          HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
	          HAL_SPI_Transmit(&hspi1, (uint8_t *)&EEPROM_READ, 1, 100);
	          HAL_SPI_Transmit(&hspi1, (uint8_t *)&addr, 1, 100);
	          HAL_SPI_Receive_IT(&hspi1, (uint8_t *)spi_buf, 8);

	          // Go to next state: waiting for receive to finish
	          state += 1;

	          break;

	        // Wait for receive flag
	        case 4:

	          if (spi_recv_flag)
	          {
	            // Clear flag and go to next state
	            spi_recv_flag = 0;
	            state += 1;
	          }

	          break;

	        // Print out received bytes and wait before retransmitting
	        case 5:

	          // Print out bytes
	          print("movement has been detected! the time is %s",spi_buf);
	          HAL_Delay(1000);
	          state = 3;

	          break;
	        default:
	          break;
	      }

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin : PB6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}



/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

