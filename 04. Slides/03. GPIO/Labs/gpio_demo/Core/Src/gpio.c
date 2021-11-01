/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins
*/
void MX_GPIO_Init(void)
{

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOD);

  /**/
  LL_GPIO_ResetOutputPin(LED01_GPIO_Port, LED01_Pin);

  /**/
  GPIO_InitStruct.Pin = USER_BTN_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(USER_BTN_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LED01_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(LED01_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */
#define USER_LED01_PIN        12
#define USER_BTN_PIN          0

#if 0
void User_LED_Control(uint8_t state)
{
	if (state != 0)
	{
		LED01_GPIO_Port->ODR |= 1UL << USER_LED01_PIN; // UL = Unsigned Long = 32-bit
	}else {
		LED01_GPIO_Port->ODR &= ~(1UL << USER_LED01_PIN);
	}
}
#else
void User_LED_Control(uint8_t state)
{
	if (state != 0)
	{
		LL_GPIO_SetOutputPin(LED01_GPIO_Port, LED01_Pin);
	}else {
		LL_GPIO_ResetOutputPin(LED01_GPIO_Port, LED01_Pin);
	}
}
#endif

uint8_t User_CheckButton()
{
	return ( LED01_GPIO_Port->IDR & (1UL << USER_BTN_PIN) );
}

void User_LED_BlinkTest()
{
	uint16_t i = 0;
	if ( User_CheckButton() )
	{
		for (i = 0; i < 10; i++)
		{
			User_LED_Control(1); // Turn-ON
			HAL_Delay(100);
			User_LED_Control(0); // Turn-OFF
			HAL_Delay(100);
		}
	}
}
/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
