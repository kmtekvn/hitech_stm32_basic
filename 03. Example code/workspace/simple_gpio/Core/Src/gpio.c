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
  LL_GPIO_SetOutputPin(LD1_GPIO_Port, LD1_Pin);

  /**/
  LL_GPIO_ResetOutputPin(GPIOD, LD2_Pin|LD3_Pin|LD4_Pin);

  /**/
  GPIO_InitStruct.Pin = BTN_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(BTN_GPIO_Port, &GPIO_InitStruct);

  /**/
  GPIO_InitStruct.Pin = LD1_Pin|LD2_Pin|LD3_Pin|LD4_Pin;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  LL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */
/* Access Bit Set/Reset Register */
void Main_Blink_LED_01(void)
{
	LL_GPIO_SetOutputPin(LD1_GPIO_Port, LD1_Pin); /* Output high */
	HAL_Delay(500);
	LL_GPIO_ResetOutputPin(LD1_GPIO_Port, LD1_Pin); /* Output Low */
	HAL_Delay(500);
}

#define  LD1_INDEX_PIN   12

/* Write ODR Register */
void Main_Blink_LED_02(void)
{
	static uint32_t   LedPortVal = 0x00; /* 0x00 = 0 in Hexa */
	static uint8_t    LedState = 0;

	/* Read-back ODR Register */
	LedPortVal = LL_GPIO_ReadOutputPort(LD1_GPIO_Port);

	/* Processing: toggle LED bit */
	if (LedState == 1)
	{
		LedPortVal = LedPortVal | (0x01UL << LD1_INDEX_PIN);  /* Set bit */
	}else {
		LedPortVal = LedPortVal & (~(0x01UL << LD1_INDEX_PIN)); /* Clear bit */
	}

	/* Update ODR Register */
	LL_GPIO_WriteOutputPort(LD1_GPIO_Port, LedPortVal); /* Output high */

	/* Toggle new state */
	LedState ^= 1; /* 0 ^ 1 = 1 - 1 ^ 1 = 0*/

	HAL_Delay(500);
}

/*
 * Return value: 1 => Pressed, 0 = No press
 * */

#define BTN_SW_BIT_MASK          (0x01UL << 0)

uint8_t Main_Check_Btn(void)
{
	uint8_t  BtnState = 0;
	uint32_t volatile BtnPortValue = LL_GPIO_ReadInputPort(GPIOA); /* volatile -> force update to RAM, no cached */

	BtnState = BtnPortValue & BTN_SW_BIT_MASK;

	return BtnState;
}
/* USER CODE END 2 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
