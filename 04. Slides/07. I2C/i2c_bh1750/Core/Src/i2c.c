/**
  ******************************************************************************
  * @file    i2c.c
  * @brief   This file provides code for the configuration
  *          of the I2C instances.
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
#include "i2c.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

I2C_HandleTypeDef hi2c3;

/* I2C3 init function */
void MX_I2C3_Init(void)
{

  /* USER CODE BEGIN I2C3_Init 0 */

  /* USER CODE END I2C3_Init 0 */

  /* USER CODE BEGIN I2C3_Init 1 */

  /* USER CODE END I2C3_Init 1 */
  hi2c3.Instance = I2C3;
  hi2c3.Init.ClockSpeed = 100000;
  hi2c3.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c3.Init.OwnAddress1 = 0;
  hi2c3.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c3.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c3.Init.OwnAddress2 = 0;
  hi2c3.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c3.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C3_Init 2 */

  /* USER CODE END I2C3_Init 2 */

}

void HAL_I2C_MspInit(I2C_HandleTypeDef* i2cHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(i2cHandle->Instance==I2C3)
  {
  /* USER CODE BEGIN I2C3_MspInit 0 */

  /* USER CODE END I2C3_MspInit 0 */

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**I2C3 GPIO Configuration
    PC9     ------> I2C3_SDA
    PA8     ------> I2C3_SCL
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* I2C3 clock enable */
    __HAL_RCC_I2C3_CLK_ENABLE();

    /* I2C3 interrupt Init */
    HAL_NVIC_SetPriority(I2C3_EV_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(I2C3_EV_IRQn);
    HAL_NVIC_SetPriority(I2C3_ER_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(I2C3_ER_IRQn);
  /* USER CODE BEGIN I2C3_MspInit 1 */

  /* USER CODE END I2C3_MspInit 1 */
  }
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* i2cHandle)
{

  if(i2cHandle->Instance==I2C3)
  {
  /* USER CODE BEGIN I2C3_MspDeInit 0 */

  /* USER CODE END I2C3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_I2C3_CLK_DISABLE();

    /**I2C3 GPIO Configuration
    PC9     ------> I2C3_SDA
    PA8     ------> I2C3_SCL
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_9);

    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_8);

    /* I2C3 interrupt Deinit */
    HAL_NVIC_DisableIRQ(I2C3_EV_IRQn);
    HAL_NVIC_DisableIRQ(I2C3_ER_IRQn);
  /* USER CODE BEGIN I2C3_MspDeInit 1 */

  /* USER CODE END I2C3_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
#define       	  BH1750_MAX_BUFFER_LEN      	 5UL
#define           BH1750_I2C_ADDR           	 0x23UL /* ADDR = L 0010 0011*/
#define           BH1750_DEFAULT_I2C_TIMEOUT     0xFFFFFFFFUL /* Max timeout */


#define           BH1750_REG_POWER_ON            0x01UL /*0000_0001 = 0x01*/
#define           BH1750_REG_RESET               0x07UL /*0000_0111 = 0x07*/
#define           BH1750_REG_CONT_MEAS           0x10UL /*0001_0000 = 0x10*/


#define           I2C_ERROR_CHECK(err)           do {  \
													 if ( err != HAL_OK ) {  \
													 while(1){ asm("NOP"); } } \
												 } while(0);

static uint16_t  lux_buffer[BH1750_MAX_BUFFER_LEN];

static uint8_t _bh1750_write_opcode(uint8_t opcode)
{
	uint8_t  adddr_frame = BH1750_I2C_ADDR << 1 ; /*Bit 0 = 0 (WRITE) */

	return (HAL_I2C_Master_Transmit(&hi2c3, adddr_frame, &opcode, 1, BH1750_DEFAULT_I2C_TIMEOUT) );
}

static uint8_t _bh1750_read_measurement(uint16_t * ret_data)
{
	uint8_t xRet = 0;
	uint8_t  raw_data[2] = {0}; /* High byte + low byte */

	uint8_t  adddr_frame = BH1750_I2C_ADDR << 1 | 0x01; /*Bit 0 = 1 (READ) */

	xRet = HAL_I2C_Master_Receive(&hi2c3, adddr_frame, raw_data, 2, BH1750_DEFAULT_I2C_TIMEOUT);

	if ( xRet == HAL_OK ){
		*ret_data = (uint16_t)(raw_data[0] << 8 | raw_data[1]);
	}
	return xRet;
}

static void bh1750_init(void)
{
  /* Power-on sensor */
 I2C_ERROR_CHECK( _bh1750_write_opcode(BH1750_REG_POWER_ON) );

 /* Reset to clear all reg */
 I2C_ERROR_CHECK( _bh1750_write_opcode(BH1750_REG_RESET) );
}

static void bh1750_start_continious(void)
{
  I2C_ERROR_CHECK( _bh1750_write_opcode(BH1750_REG_CONT_MEAS) );
}

static void bh1750_powerdown(void)
{
  /* Un-implemented */
}

void bh1750_run_test(void)
{
	uint16_t  i =0;
	HAL_Delay(100);

	bh1750_init();
	bh1750_start_continious();
	HAL_Delay(100);

	for (i = 0; i < BH1750_MAX_BUFFER_LEN; i++)
	{
		if ( _bh1750_read_measurement(&lux_buffer[i]) )
		{
			i++;
		}
		HAL_Delay(130); /* H-Resolution Mode 120 ms */
	}

	bh1750_powerdown();
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
