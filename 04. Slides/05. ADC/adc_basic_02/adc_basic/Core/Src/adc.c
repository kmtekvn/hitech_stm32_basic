/**
  ******************************************************************************
  * @file    adc.c
  * @brief   This file provides code for the configuration
  *          of the ADC instances.
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
#include "adc.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* ADC1 init function */
void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  LL_ADC_InitTypeDef ADC_InitStruct = {0};
  LL_ADC_REG_InitTypeDef ADC_REG_InitStruct = {0};
  LL_ADC_CommonInitTypeDef ADC_CommonInitStruct = {0};

  /* Peripheral clock enable */
  LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);

  /* ADC1 DMA Init */

  /* ADC1 Init */
  LL_DMA_SetChannelSelection(DMA2, LL_DMA_STREAM_0, LL_DMA_CHANNEL_0);

  LL_DMA_SetDataTransferDirection(DMA2, LL_DMA_STREAM_0, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

  LL_DMA_SetStreamPriorityLevel(DMA2, LL_DMA_STREAM_0, LL_DMA_PRIORITY_MEDIUM);

  LL_DMA_SetMode(DMA2, LL_DMA_STREAM_0, LL_DMA_MODE_CIRCULAR);

  LL_DMA_SetPeriphIncMode(DMA2, LL_DMA_STREAM_0, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA2, LL_DMA_STREAM_0, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA2, LL_DMA_STREAM_0, LL_DMA_PDATAALIGN_HALFWORD);

  LL_DMA_SetMemorySize(DMA2, LL_DMA_STREAM_0, LL_DMA_MDATAALIGN_HALFWORD);

  LL_DMA_DisableFifoMode(DMA2, LL_DMA_STREAM_0);

  /* ADC1 interrupt Init */
  NVIC_SetPriority(ADC_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 0));
  NVIC_EnableIRQ(ADC_IRQn);

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Common config
  */
  ADC_InitStruct.Resolution = LL_ADC_RESOLUTION_12B;
  ADC_InitStruct.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
  ADC_InitStruct.SequencersScanMode = LL_ADC_SEQ_SCAN_DISABLE;
  LL_ADC_Init(ADC1, &ADC_InitStruct);
  ADC_REG_InitStruct.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
  ADC_REG_InitStruct.SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE;
  ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
  ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
  ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_LIMITED;
  LL_ADC_REG_Init(ADC1, &ADC_REG_InitStruct);
  LL_ADC_REG_SetFlagEndOfConversion(ADC1, LL_ADC_REG_FLAG_EOC_UNITARY_CONV);
  ADC_CommonInitStruct.CommonClock = LL_ADC_CLOCK_SYNC_PCLK_DIV2;
  ADC_CommonInitStruct.Multimode = LL_ADC_MULTI_INDEPENDENT;
  LL_ADC_CommonInit(__LL_ADC_COMMON_INSTANCE(ADC1), &ADC_CommonInitStruct);
  /** Configure Regular Channel
  */
  LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, LL_ADC_CHANNEL_VREFINT);
  LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_VREFINT, LL_ADC_SAMPLINGTIME_3CYCLES);
  LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1), LL_ADC_PATH_INTERNAL_VREFINT);
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/* USER CODE BEGIN 1 */
#define  ADC_USING_DMA     1
#define  USER_ADC_INST     ADC1
#define  ADC_RATIO     	   0.733 /* Vref/(2^n - 1) mV <=> 3000 / 4095 = 0.733*/

static uint32_t  adc_val = 0;
static uint32_t  volt_val = 0;
volatile uint8_t adc_conv_done = 0;

uint16_t adc_read_conversion()
{
	/* Turn-ON ADC */
	LL_ADC_Enable(USER_ADC_INST);

	/* Enable EOC interrupt */
	LL_ADC_EnableIT_EOCS(USER_ADC_INST);

	/* Tstab - wait ADC hardware stable */
	HAL_Delay(10); // 10 ms

	/* Trigger conversion */
	LL_ADC_REG_StartConversionSWStart(USER_ADC_INST);

	/* Blocking until EOC Interrupt occurs */
	while ( !adc_conv_done) { };

	/* Read ADC Value */
	return ( LL_ADC_REG_ReadConversionData12(USER_ADC_INST) );
}

uint16_t adc_read_conversion_using_dma()
{
	/* Turn-ON ADC */
	LL_ADC_Enable(USER_ADC_INST);

	/* Tstab - wait ADC hardware stable */
	HAL_Delay(10); // 10 ms

	/* Setup DMA transaction */
	LL_ADC_REG_SetDMATransfer(USER_ADC_INST, LL_ADC_REG_DMA_TRANSFER_LIMITED);
	LL_DMA_SetPeriphAddress(DMA2, LL_DMA_STREAM_0, (uint32_t)LL_ADC_DMA_GetRegAddr(USER_ADC_INST, LL_ADC_DMA_REG_REGULAR_DATA) );
	LL_DMA_SetMemoryAddress(DMA2, LL_DMA_STREAM_0, (uint32_t)&adc_val);
	LL_DMA_SetDataLength(DMA2, LL_DMA_STREAM_0, 1);

	LL_DMA_EnableIT_TC(DMA2, LL_DMA_STREAM_0);

	LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_0);
	/* Trigger conversion */
	LL_ADC_REG_StartConversionSWStart(USER_ADC_INST);
}


uint32_t adc_voltage_conversion(uint16_t raw_data)
{
	return (uint32_t)((float)ADC_RATIO * (float)raw_data);
}

void adc_unitest()
{
#if ADC_USING_DMA > 0
	adc_read_conversion_using_dma();
#else
#define  ADC_NUM_SAMPLES  10
	uint32_t i = 0;
	for (i=0; i<ADC_NUM_SAMPLES; i++)
	{
		adc_val += adc_read_conversion();
	}

	volt_val = adc_voltage_conversion((uint16_t)(adc_val/ADC_NUM_SAMPLES));
	asm("NOP");
#endif
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
