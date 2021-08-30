#include "common.h"
#include "sw_rtc.h"

#include "main.h"

#define   MAX_LED_OUTPUT		3
#define   LED_BASE_IDX          12
#define   LED_OUTPUT_PORT       GPIOD

static uint32_t volatile msTicks = 0;                              /* Variable to store millisecond ticks */
static time_t   currTime = {0};
static uint16_t ledIndPinIdx = 0;

/* LED output indicator */
void sw_rtc_led_ind (void) {
	uint32_t   gpioPinMask = 0;

	/* Turn-off all LEDS*/
	if (ledIndPinIdx >= MAX_LED_OUTPUT)
	{
		/* Read current outputs REG */
		gpioPinMask = LL_GPIO_ReadOutputPort(LED_OUTPUT_PORT);
		/* Create bit mask to reset bit from D12 -> D14 */
		gpioPinMask = gpioPinMask & ~(0x07 << LED_BASE_IDX);
		/* Update new value to GPIO OUTPUT REG*/
		LL_GPIO_WriteOutputPort(LED_OUTPUT_PORT, gpioPinMask);
		ledIndPinIdx = 0;
	}else {
		gpioPinMask = 0x01 << (ledIndPinIdx++ + LED_BASE_IDX);
		LL_GPIO_SetOutputPin(LED_OUTPUT_PORT, gpioPinMask);
	}
}

/* Main software RTC implementation */
int sw_rtc_app_run (void)  {
	SysTick_Config (SystemCoreClock / 1000);
	
  while(1)
	{
		HAL_Delay(500);
		sw_rtc_gettime( &currTime );
		sw_rtc_led_ind();
	}
}

 /* SysTick interrupt Handler. */
void sw_rtc_systick_update(void)
{                              
    msTicks++;
	if (msTicks >= SW_RTC_TICK_PER_SECOND)
	{
		msTicks = 0;
		sw_rtc_update();
	}
}
