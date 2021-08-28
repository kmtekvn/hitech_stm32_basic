#include "common.h"
#include "sw_rtc.h"

#include "main.h"

static uint32_t volatile msTicks = 0;                              /* Variable to store millisecond ticks */
static time_t   currTime = {0};

int sw_rtc_app_run (void)  {
	SysTick_Config (SystemCoreClock / 1000);
	
  while(1)
	{
		HAL_Delay(500);
		sw_rtc_gettime( &currTime );
		LL_GPIO_TogglePin(LD1_GPIO_Port, LD1_Pin);
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
