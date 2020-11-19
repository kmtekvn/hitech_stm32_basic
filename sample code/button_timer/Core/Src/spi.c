/**
  ******************************************************************************
  * File Name          : SPI.c
  * Description        : This file provides code for the configuration
  *                      of the SPI instances.
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
#include "spi.h"

/* USER CODE BEGIN 0 */
#include "main.h"
#include "stm32f4xx_ll_spi.h"

/* USER CODE END 0 */

/* SPI1 init function */
void MX_SPI1_Init(void)
{
  LL_SPI_InitTypeDef SPI_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_SPI1);

  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
  /**SPI1 GPIO Configuration
  PA5   ------> SPI1_SCK
  PA6   ------> SPI1_MISO
  PA7   ------> SPI1_MOSI
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_5|LL_GPIO_PIN_6|LL_GPIO_PIN_7;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_5;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_HIGH;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_2EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV32;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly = 10;
  LL_SPI_Init(SPI1, &SPI_InitStruct);
  LL_SPI_SetStandard(SPI1, LL_SPI_PROTOCOL_MOTOROLA);

}

/* USER CODE BEGIN 1 */
#define ACC_SPI_INST      SPI1

void _spi_enable(void)
{
	LL_SPI_Enable(ACC_SPI_INST);
	LL_SPI_SetCRCPolynomial(ACC_SPI_INST, 00);
}

void _spi_cs_set(void) 
{
  HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_SET);
}

void _spi_cs_reset(void)
{
  HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, GPIO_PIN_RESET);
}

uint16_t __spi_write_byte(uint8_t data)
{

	while(LL_I2S_IsActiveFlag_BSY(ACC_SPI_INST) == SET) {};
	
	LL_SPI_TransmitData8(ACC_SPI_INST, data);
	while(LL_I2S_IsActiveFlag_BSY(ACC_SPI_INST) == SET) {};
	
	while(LL_SPI_IsActiveFlag_RXNE(ACC_SPI_INST) == RESET) {};
			
	return LL_SPI_ReceiveData8(ACC_SPI_INST);
}

void _spi_write_reg(uint8_t reg, uint8_t data) 
{
	_spi_cs_reset();
	
	__spi_write_byte(reg);
	__spi_write_byte(data);
	
	_spi_cs_set();
}

/* Doc gia tri thanh ghi qua bus SPI*/
uint8_t _spi_read_reg(uint8_t reg) 
{
  uint8_t read_reg = 0;
	uint8_t data = 0;
	
	_spi_cs_reset();
	
	read_reg = reg | 0x80; /* Set bit MSB (bit 7) 0: Write, 1:Read */
	
	__spi_write_byte(read_reg);
	
	data = __spi_write_byte(0x00);
	
	_spi_cs_set();
	
	return data;

}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
