/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

UART_HandleTypeDef huart2;

/* USART2 init function */

void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspInit 0 */

  /* USER CODE END USART2_MspInit 0 */
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspInit 1 */

  /* USER CODE END USART2_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART2)
  {
  /* USER CODE BEGIN USART2_MspDeInit 0 */

  /* USER CODE END USART2_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();

    /**USART2 GPIO Configuration
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  /* USER CODE BEGIN USART2_MspDeInit 1 */

  /* USER CODE END USART2_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
#define TEST_UART     huart2  /* Macro dinh nghia khoi UART dang su dung*/
#include "stm32f4xx_hal_usart.h"

static uint8_t recv_data = 0;
/*  Vi du truyen & nhan 1 byte data */
void uart_send_and_receive(uint8_t send_data)
{
	//==============TRUYEN DU LIEU===========
	/*Step 1:  wait TXE=1 => DR empty - Trang 1007 Ref manual */
	  while(__HAL_USART_GET_FLAG(&TEST_UART, USART_FLAG_TXE) == 0) {};
    // {} -> them vao de compilier ko tối ưu code,
    // dan den ko compile lệnh này
			
	/*Step 2: Ghi vao DR*/
	 TEST_UART.Instance->DR = (uint32_t)(send_data & 0xFF);
	/*Step 3: cho den khi TC = 1 - Trang 1007 Ref manual*/
   while(__HAL_USART_GET_FLAG(&TEST_UART, USART_FLAG_TC) == 0) {};
		 
  //==============NHAN DU LIEU===========
		 /* Step 3: cho Flag RXNE (RX Not Empty) raise 1 -> doc du lieu nhan duoc tu thanh ghi DR*/
  while(__HAL_USART_GET_FLAG(&TEST_UART, USART_FLAG_RXNE) == 0) {};
  recv_data = (uint8_t) ( (TEST_UART.Instance->DR) & 0xFF );
}


#include <stdio.h>  /* vsnprintf*/
#include <stdarg.h> /* Xu ly argument co nhieu doi so*/

#define MAX_DEBUG_BUF_LEN  512
static char debug_buf[MAX_DEBUG_BUF_LEN];

void __uart_putchar(char c)
{
		//==============TRUYEN DU LIEU===========
	/*Step 1:  wait TXE=1 => DR empty */
	  while(__HAL_USART_GET_FLAG(&TEST_UART, USART_FLAG_TXE) == 0) {};
    // {} -> them vao de compilier ko tối ưu code,
    // dan den ko compile lệnh này
	/*Step 2: Ghi vao DR*/
	 TEST_UART.Instance->DR = (uint32_t)(c & 0xFF);
	/*Step 3: cho den khi TC = 1*/
   while(__HAL_USART_GET_FLAG(&TEST_UART, USART_FLAG_TC) == 0) {};
}

void __sendDebugToUart(const char* msg)
{
	  /* Lap den khi nao ket thuc chuoi, va kiem tra pointer NOT NULL*/
		while(msg && *msg)
		{
				__uart_putchar(*msg++);
		}
}

void __sendBufferToUart(const char* msg, uint32_t len)
{
	  /* Lap den khi nao ket thuc chuoi, va kiem tra pointer NOT NULL*/
		while(len--)
		{
				__uart_putchar(*msg++);
		}
}

void __sendDebugMsg ( const char * format, ... )
{
  va_list args;
	
	/* Tao chuoi tu format nhieu doi so*/
  va_start (args, format);
  vsnprintf (debug_buf, MAX_DEBUG_BUF_LEN, format, args);
	
	/* Gui chuoi debug ra UART */
	__sendDebugToUart(debug_buf);
	
  va_end (args);
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
