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
#define ADC_CONV_RATIO            0.7326  /*Vref / (2^n - 1) = 3000 / 4095 = 0.7326*/

volatile uint8_t  adc_conv_done = 0;


/* Union - bit field */
typedef union {
	struct {
		uint8_t run:1;
		uint8_t idle:1;
		uint8_t reserve:6;
	} flags;
	uint8_t status;
} DeviceStatus_t;

/* Union - endian */

typedef union {
	struct {
		uint8_t  byte_field_01;
		uint8_t  byte_field_02;
		uint8_t  byte_field_03;
		uint8_t  byte_field_04;
	} data_array;
	uint32_t  user_data;
} EndianCheck_t;


void union_bitfield_example()
{
  static DeviceStatus_t  myStatus = 0x01; /* Flag Idle = 1 */

  /* Change run -> stop */
  //status = 0x02;
  myStatus.flags.run = 1;
  myStatus.flags.idle = 0;
}

void uinion_endian_example()
{
  static EndianCheck_t  myEdian = 0xABCDBEEF;

  myEdian.data_array.byte_field_01 = 0xDD;

}
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
  ADC_REG_InitStruct.ContinuousMode = LL_ADC_REG_CONV_CONTINUOUS;
  ADC_REG_InitStruct.DMATransfer = LL_ADC_REG_DMA_TRANSFER_UNLIMITED;
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
#define  ADC_USE_DMA    (1)

#if ADC_USE_DMA > 0

#define   ADC_SAMPLE_BUFFER_LEN      32UL
static uint16_t adc_convert_data[ADC_SAMPLE_BUFFER_LEN] = { 0 };

static uint16_t __adc_calc_average(uint16_t* in_buf)
{

}

uint16_t __adc_single_convert(void)
{
	/*B1. Turn-on ADON */
	LL_ADC_Enable(ADC1);
	/*B2. Stable time */
	HAL_Delay(10);

	/* Config ADC DMA */
	/* Enable DMA Request */
	LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);

	/* Config Peripheral address - ADC Data Register  */
	LL_DMA_SetPeriphAddress( DMA2, LL_DMA_STREAM_0,  LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA) );

	/* Config Memory Address */
	LL_DMA_SetMemoryAddress( DMA2, LL_DMA_STREAM_0, (uint32_t)(&adc_convert_data) );

	/* Number of transfer bytes */
	LL_DMA_SetDataLength( DMA2, LL_DMA_STREAM_0, ADC_SAMPLE_BUFFER_LEN);

	/* Enable DMA Stream */
	LL_DMA_EnableStream( DMA2, LL_DMA_STREAM_0 );

	/*B3. Set SWSTART */
	LL_ADC_REG_StartConversionSWStart(ADC1);

	/* Waiting unitl DMA transfer complete */
	//while ( !LL_DMA_IsActiveFlag_TC0(DMA2) ) { };
	LL_DMA_EnableIT_TC( DMA2, LL_DMA_STREAM_0 );

	/* Calculate average */
	return __adc_calc_average(adc_convert_data);
}

#else
uint16_t __adc_single_convert(void)
{
	/*B1. Turn-on ADON */
	LL_ADC_Enable(ADC1);

	/* Enable EOC Interrupt */
	LL_ADC_EnableIT_EOCS(ADC1);

	/*B2. Stable time */
	HAL_Delay(10);

	/*B3. Set SWSTART */
	LL_ADC_REG_StartConversionSWStart(ADC1);

	/* Waiting conversion complete */
	while(!adc_conv_done) { };

	/* Read ADC Data register */
	return (uint16_t)( LL_ADC_REG_ReadConversionData32(ADC1) );
}
#endif

void adc_demo_test(void)
{
	static uint16_t  adc_raw = 0;
	static uint16_t  adc_v = 0;

	union_bitfield_example();
	uinion_endian_example();

	adc_raw = __adc_single_convert();
	adc_v = (float)(adc_raw) * (float)(ADC_CONV_RATIO);
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
