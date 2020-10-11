/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define DEBOUCING_TIMER_INST    htim2
#define DEBOUCING_VALID_STATE   1

static __IO uint32_t _debounced_counter = 0;
static uint16_t _active_debouced_pin = 0;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define GPIOD_ADDRESS  (0x40020C00) //Dia chi bat dau vung nho thuoc GPIOD
#define BSRR_OFFSET    (0x18) // Dia chi thanh ghi Bit Set/Reset
#define BODR_OFFSET    (0x14) //Dia chi thanh ghi luu trang thai output

#define BSRR_ABS_ADDRESS   (GPIOD_ADDRESS + BSRR_OFFSET)  // Tinh dia chi vat ly thanh ghi Bit Set/ReseOutput
#define BODR_ABS_ADDRESS   (GPIOD_ADDRESS + BODR_OFFSET)  // Tinh dia chi vat ly thanh ghi Output

typedef  unsigned int  my_uint32_t;  // Dinh nghia lai kieu du lieu 32-bit interger

// Tao bien con tro toi thanh ghi bsrr
static my_uint32_t*  bsrr_reg_pointer = (uint32_t*)(BSRR_ABS_ADDRESS); // Tao bien con tro toi vung  nho thanh ghi BSRR
static my_uint32_t*  bodr_reg_pointer = (uint32_t*)(BODR_ABS_ADDRESS); // Tao bien con tro toi vung  nho thanh ghi ODR

static my_uint32_t bsrr_value = 0; // Bien luu gia tri thanh ghi BSRR
static my_uint32_t bodr_value = 0; // Bien luu gia tri thanh ghi ODR

static uint8_t led_state =  0; // Khai bao bien interger 8-bit luu trang thai LED

/* Tao macro port va pin cua chan DK LED */
#define   MY_LED_PORT  		GPIOD
#define   MY_LED_PIN  		GPIO_PIN_12
		
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define USER_USING_STD_LIB  0
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void control_led_using_stdlib(void)
{
		led_state = HAL_GPIO_ReadPin(MY_LED_PORT, MY_LED_PIN);
		HAL_GPIO_WritePin(MY_LED_PORT, MY_LED_PIN, !led_state);	
}

void control_led_using_pointer(void)
{
	// Doc gia tri thanh ghi output
	bodr_value = *(bodr_reg_pointer);
	// Xuat nguoc gia tri output
	*(bsrr_reg_pointer) = ~bodr_value;
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM2_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
#if USER_USING_STD_LIB
		control_led_using_stdlib();
#else
		control_led_using_pointer();
#endif		/* USER_USING_STD_LIB */	
		HAL_Delay(1000); // Tao tre 1s
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void USER_DEBOUNCE_TMR_IRQHandler(void)
{
	/* Stop deboucing timer */
	HAL_TIM_Base_Stop_IT(&DEBOUCING_TIMER_INST);
	
	/* Check if deboucing pin is in valid state */
	if (HAL_GPIO_ReadPin(USER_BTN_GPIO_Port, _active_debouced_pin) == DEBOUCING_VALID_STATE){
		_debounced_counter += 1;
	}
}

void USER_EXTI_IRQHandler(uint16_t pinNo)
{
	/* Save active GPIO Pin*/
	_active_debouced_pin = pinNo;
	
	/* Start debounce timer */
	HAL_TIM_Base_Start_IT(&DEBOUCING_TIMER_INST);
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
