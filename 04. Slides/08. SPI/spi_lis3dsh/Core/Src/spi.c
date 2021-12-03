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
#define SPI_CS_LOW()       do { HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, 0); } while(0);
#define SPI_CS_HIGH()      do { HAL_GPIO_WritePin(SPI1_CS_GPIO_Port, SPI1_CS_Pin, 1); } while(0);


uint8_t __spi_xfer_data(uint8_t data)
{
	LL_SPI_TransmitData8(SPI1, data);
	/* Wait TXE Empty */
	while( !LL_SPI_IsActiveFlag_TXE(SPI1) ) { };

	/* Wait complete receive 1 byte */
	while( !LL_SPI_IsActiveFlag_RXNE(SPI1) ) { };

	return ( LL_SPI_ReceiveData8(SPI1) );
}

uint8_t spi_read_reg(uint8_t reg_addr)
{
   uint8_t frame = 0;
   SPI_CS_LOW();

   /* Send address */
   frame = 0x80 | reg_addr;
   __spi_xfer_data(frame);

   /* Send dummy data & get sensor data */
   frame = __spi_xfer_data(0x00);

   SPI_CS_HIGH();

   return frame;
}

uint8_t spi_write_reg(uint8_t reg_addr, uint8_t reg_val)
{
	   SPI_CS_LOW();

	   /* Send address */
	   reg_addr =  reg_addr & 0x7F;
	   __spi_xfer_data(reg_addr);

	   /* Send dummy data & get sensor data */
	   __spi_xfer_data(reg_val);

	   SPI_CS_HIGH();
}

#define  LIS3DSH_WHOAMI_REG      0x0F
#define  LIS3DSH_CR1_REG         0x20
#define  LIS3DSH_OUT_X_REG_H       0x29
#define  LIS3DSH_OUT_X_REG_L       0x2B

void spi_unittest(void)
{
	static uint8_t dev_id = 0xFF;
	static uint8_t out_x_h = 0x00;
	static uint8_t out_x_l = 0x00;

	/* Enable SPI */
	LL_SPI_Enable(SPI1);

	dev_id = spi_read_reg(LIS3DSH_WHOAMI_REG);

	/* Config CR1 register: 0100 0001 = 0x41*/
	spi_write_reg(LIS3DSH_CR1_REG, 0x41);

	while (1)
	{
		out_x_h = spi_read_reg(LIS3DSH_OUT_X_REG_H);
		out_x_l = spi_read_reg(LIS3DSH_OUT_X_REG_L);
		HAL_Delay(100);
	}
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
