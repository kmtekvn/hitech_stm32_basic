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
void sw_rtc_led_show_pwm (void) {
	uint32_t currPwmSetting = 0;

//	/* Read current pwm duty */
//	currPwmSetting = LL_TIM_OC_GetCompareCH4();
//
//	/* Calculate new duty */
//	currPwmSetting += 10;
//
//	/* Update new setting */
//	LL_TIM_OC_SetCompareCH4(currPwmSetting);
}

/* Main software RTC implementation */
int sw_rtc_app_run (void)  {
	SysTick_Config (SystemCoreClock / 1000); /* 1mS */
	
  while(1)
	{
		HAL_Delay(500);
		sw_rtc_gettime( &currTime );
		sw_rtc_led_show_pwm();
	}
}

 /*  */
void sw_rtc_systick_update(void) {
    msTicks++;
	if (msTicks >= SW_RTC_TICK_PER_SECOND)
	{
		msTicks = 0;
		sw_rtc_update();
	}
}

