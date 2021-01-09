/**
  ******************************************************************************
  * File Name          : ADC.c
  * Description        : This file provides code for the configuration
  *                      of the ADC instances.
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
#include "adc.h"

/* USER CODE BEGIN 0 */
#include "stm32f4xx_ll_adc.h"

/* USER CODE END 0 */

ADC_HandleTypeDef hadc1;
DMA_HandleTypeDef hdma_adc1;

/* ADC1 init function */
void MX_ADC1_Init(void)
{
  ADC_ChannelConfTypeDef sConfig = {0};

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_LEFT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspInit 0 */

  /* USER CODE END ADC1_MspInit 0 */
    /* ADC1 clock enable */
    __HAL_RCC_ADC1_CLK_ENABLE();

    /* ADC1 DMA Init */
    /* ADC1 Init */
    hdma_adc1.Instance = DMA2_Stream0;
    hdma_adc1.Init.Channel = DMA_CHANNEL_0;
    hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode = DMA_CIRCULAR;
    hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
    hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
    if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(adcHandle,DMA_Handle,hdma_adc1);

    /* ADC1 interrupt Init */
    HAL_NVIC_SetPriority(ADC_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(ADC_IRQn);
  /* USER CODE BEGIN ADC1_MspInit 1 */
    LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);
  /* USER CODE END ADC1_MspInit 1 */
  }
}

void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

  if(adcHandle->Instance==ADC1)
  {
  /* USER CODE BEGIN ADC1_MspDeInit 0 */

  /* USER CODE END ADC1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_ADC1_CLK_DISABLE();

    /* ADC1 DMA DeInit */
    HAL_DMA_DeInit(adcHandle->DMA_Handle);

    /* ADC1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(ADC_IRQn);
  /* USER CODE BEGIN ADC1_MspDeInit 1 */

  /* USER CODE END ADC1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
static uint16_t target_adc_mem = 0;



void adc_start_converting(void)
{
  /*1. Start ADC conversion */
  HAL_ADC_Start(&hadc1);

  /*2. Waiting operation complete */
  while(!__HAL_ADC_GET_FLAG(&hadc1, ADC_FLAG_EOC)) {};
}

void adc_dma_start_transfer(void)
{
  /*1. Config source peripheral address */
  uint32_t src_addr = (uint32_t)(&((hadc1.Instance)->DR));
  LL_DMA_SetPeriphAddress(DMA2, LL_DMA_STREAM_0,  src_addr);

	
  /*2. Config destination RAM address */
	uint32_t dst_addr = (uint32_t)(&target_adc_mem);
   LL_DMA_SetMemoryAddress(DMA2, LL_DMA_STREAM_0, dst_addr);

	  /* 3Set DMA transfer size */
  LL_DMA_SetDataLength(DMA2,
                        LL_DMA_STREAM_0,
                       1);
	
  /*4. Enable transfer */
   LL_DMA_EnableStream(DMA2, LL_DMA_STREAM_0);

  /*5. Waiting operation complete (replace by interrupt??) */
   while(!LL_DMA_IsActiveFlag_TC0(DMA2)) {

   }
}

#define ADC_VOLT_CONV_RATIO    (16.0)
#define ADC_UNIT_RATIO         (10000UL) // UL: unsigned long - 32-bit

#define ADC_QUEUE_LEN          (128)
#define ADC_QUEUE_ITEM_SIZE    sizeof(uint32_t)
	
QueueHandle_t  xTempADCQueue = NULL;

float __adc_convert_to_voltage(uint32_t raw_data)
{
	 return ( (float)( ((float)raw_data/(float)ADC_VOLT_CONV_RATIO)) );
}

float adc_convert_to_temperature(uint32_t adc_in)
{
	//Temperature (in °C) = {(VSENSE – V25) / Avg_Slope} + 25
	uint32_t vsense = __adc_convert_to_voltage(adc_in);
	#define ADC_V_CALIB_AT_25   (0.76)
	#define ADC_AVG_SLOPE       (0.25)
	#define ADC_DEFAULT_OFFSET   (25)
	
	return (( vsense - ADC_V_CALIB_AT_25)/ADC_AVG_SLOPE) + ADC_DEFAULT_OFFSET;
}

void adc_queue_init(void)
{
		xTempADCQueue = xQueueCreate(ADC_QUEUE_LEN, ADC_QUEUE_ITEM_SIZE);
}



QueueHandle_t adc_queue_get_handle(void)
{
	return xTempADCQueue;
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
