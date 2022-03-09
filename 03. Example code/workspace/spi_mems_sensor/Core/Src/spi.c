/**
  ******************************************************************************
  * @file    spi.c
  * @brief   This file provides code for the configuration
  *          of the SPI instances.
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
#include "spi.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* SPI1 init function */
void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

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

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  SPI_InitStruct.TransferDirection = LL_SPI_FULL_DUPLEX;
  SPI_InitStruct.Mode = LL_SPI_MODE_MASTER;
  SPI_InitStruct.DataWidth = LL_SPI_DATAWIDTH_8BIT;
  SPI_InitStruct.ClockPolarity = LL_SPI_POLARITY_HIGH;
  SPI_InitStruct.ClockPhase = LL_SPI_PHASE_2EDGE;
  SPI_InitStruct.NSS = LL_SPI_NSS_SOFT;
  SPI_InitStruct.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV4;
  SPI_InitStruct.BitOrder = LL_SPI_MSB_FIRST;
  SPI_InitStruct.CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE;
  SPI_InitStruct.CRCPoly = 10;
  LL_SPI_Init(SPI1, &SPI_InitStruct);
  LL_SPI_SetStandard(SPI1, LL_SPI_PROTOCOL_MOTOROLA);
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/* USER CODE BEGIN 1 */
#define  ACC_WHOAMI_REG     0x0F

static uint8_t  sensorID = 0x00;

#define SPI_CS_LOW()   do { HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, 0); } while(0);
#define SPI_CS_HIGH()  do { HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, 1); } while(0);

void spi_sensor_init()
{
	LL_SPI_Enable(SPI1);
	HAL_Delay(100);
}

void spi_read_sensor_id()
{
	uint8_t reqFrame = 0;
	/* Assert CS = 0*/
	SPI_CS_LOW();

	/*---------------PHASE 1: SEND REQUEST------------*/
	/* Sending Fist Frame = R/W + AD6-AD0 */
	reqFrame = 0x80 | ACC_WHOAMI_REG;
	LL_SPI_TransmitData8(SPI1, reqFrame);
	while ( !LL_SPI_IsActiveFlag_RXNE(SPI1) ) { };
	/*Read out Data to clear RXNE Flag*/
	LL_SPI_ReceiveData8(SPI1);

	/*----------------PHASE 2: READ DATA---------------*/
	/*Wait TXE = 1*/
	while ( !LL_SPI_IsActiveFlag_TXE(SPI1) ) { };
	/* Master read data from slave by writing dumy frame */
	LL_SPI_TransmitData8(SPI1, 0x00); /*Second sent frame */
	/* Wait RXNE Flag is raise */
	while ( !LL_SPI_IsActiveFlag_RXNE(SPI1) ) { };
	sensorID = LL_SPI_ReceiveData8(SPI1);

	/*---------------PHASE 3: TERMINATE----------------*/
	/* Wait BSY = 0*/
	while ( LL_SPI_IsActiveFlag_BSY(SPI1) ) { };

	/* Deassert CS = 1*/
	SPI_CS_HIGH();
}

void spi_sensor_test()
{
	spi_sensor_init();
	spi_read_sensor_id();
}
/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
