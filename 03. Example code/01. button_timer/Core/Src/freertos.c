/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "acc_sensor_driver.h"
#include "queue.h"
#include <string.h>
#include "adc.h"

#define ACC_SENSOR_Q_LEN       10
#define ACC_SENSOR_Q_SIZE      (sizeof(Lis3dsh))

static QueueHandle_t acc_queue = NULL;
static Lis3dsh _fetch_data = {0};
static  void __acc_queue_create(void)
{
 acc_queue = xQueueCreate(ACC_SENSOR_Q_LEN, ACC_SENSOR_Q_SIZE);
}

static  void __acc_queue_send(Lis3dsh* sent_data)
{
	xQueueSend(acc_queue, (void*)sent_data, portMAX_DELAY);
}

static  void __acc_fetch_from_queue(Lis3dsh* out_data)
{
	static Lis3dsh _get_data = {0};
	
	if ( xQueueReceive(acc_queue, &_get_data, portMAX_DELAY) == pdPASS)
	{
		memcpy( (void*)out_data, (void*)&_get_data, sizeof(Lis3dsh));
	}
}


/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
void StartMyADCTask(void *argument);
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .priority = (osPriority_t) osPriorityNormal,
  .stack_size = 128 * 4
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void StartMyADCTask(void *argument);
void StartMyTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  

  /* USER CODE BEGIN RTOS_THREADS */
	//defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);
	osThreadNew(StartMyADCTask, NULL, &defaultTask_attributes);
	//osThreadNew(StartMyTask, NULL, &defaultTask_attributes);
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
		acc_sensor_reading();
		__acc_queue_send(acc_get_data_pointer());

    osDelay(50);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void StartMyTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
	
  for(;;)
  {
		__acc_fetch_from_queue(&_fetch_data);
		__acc_sensor_led_indicator(&_fetch_data);
		__acc_send_dataframe(&_fetch_data);
    osDelay(10);
  }
  /* USER CODE END StartDefaultTask */
}


void StartMyADCTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
	adc_queue_init();
	//HAL_ADC_Start_IT(&hadc1);

  adc_start_converting();
  adc_dma_start_transfer();
  
  /* Infinite loop */
	uint32_t fetch_data = 0;
	static float cel_c = 0;
  for(;;)
  {
		if (xQueueReceive (adc_queue_get_handle(), &fetch_data, portMAX_DELAY) == pdPASS)
		{
			cel_c = adc_convert_to_temperature(fetch_data);
		}
    osDelay(10);
  }
  /* USER CODE END StartDefaultTask */
}
/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
