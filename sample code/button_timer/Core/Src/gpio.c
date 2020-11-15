/**
  ******************************************************************************
  * File Name          : gpio.c
  * Description        : This file provides code for the configuration
  *                      of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"
/* USER CODE BEGIN 0 */
/* Tao macro port va pin cua chan DK LED */
#define   MY_LED_PORT     GPIOD
#define   MY_LED_PIN      GPIO_PIN_12
    

#define LED3_ORANGE_GPIOx  LED_ORGANGE_GPIO_Port
#define LED4_GREEN_GPIOx   LED_GREEN_GPIO_Port
#define LED5_RED_GPIOx     LED_RED_GPIO_Port
#define LED6_BLUE_GPIOx    LED_BLUE_GPIO_Port


#define LED3_ORANGE_PinNumber   LED_ORGANGE_Pin
#define LED4_GREEN_PinNumber    LED_GREEN_Pin
#define LED5_RED_PinNumber      LED_RED_Pin
#define LED6_BLUE_PinNumber     LED_BLUE_Pin

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */
static uint8_t led_state =  0; // Khai bao bien interger 8-bit luu trang thai LED

/* USER CODE END 1 */

/** Configure pins
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, LED_GREEN_Pin|LED_ORGANGE_Pin|LED_RED_Pin|LED_BLUE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = SPI1_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(SPI1_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PtPin */
  GPIO_InitStruct.Pin = USER_BTN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(USER_BTN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PDPin PDPin PDPin PDPin */
  GPIO_InitStruct.Pin = LED_GREEN_Pin|LED_ORGANGE_Pin|LED_RED_Pin|LED_BLUE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}

/* USER CODE BEGIN 2 */
void control_led_using_stdlib(void)
{
		led_state = HAL_GPIO_ReadPin(MY_LED_PORT, MY_LED_PIN);
		HAL_GPIO_WritePin(MY_LED_PORT, MY_LED_PIN, !led_state);	
}


void gpio_led_state(uint8_t LED_ID, uint8_t state)
{
	GPIO_PinState bitValue;
	bitValue = (state == 1) ? GPIO_PIN_SET : GPIO_PIN_RESET;
	switch(LED_ID)
	{
		case LED3_ORANGE_ID:
			HAL_GPIO_WritePin(LED3_ORANGE_GPIOx, LED3_ORANGE_PinNumber, bitValue);
		break;
		case LED4_GREEN_ID:
			HAL_GPIO_WritePin(LED4_GREEN_GPIOx, LED4_GREEN_PinNumber, bitValue);
			break;
		case LED5_RED_ID:
			HAL_GPIO_WritePin(LED5_RED_GPIOx, LED5_RED_PinNumber, bitValue);
			break;
		case LED6_BLUE_ID:
				HAL_GPIO_WritePin(LED6_BLUE_GPIOx, LED6_BLUE_PinNumber, bitValue);
			break;
	}
}
/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
