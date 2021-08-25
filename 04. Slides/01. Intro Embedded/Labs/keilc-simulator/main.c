#include "ARMCM4.h"
#include "common.h"
#include "sw_rtc.h"

static uint32_t volatile msTicks = 0;                              /* Variable to store millisecond ticks */
static time_t   currTime = {0};

static void _sw_delay_ms( uint32_t period );

int main (void)  {
	SysTick_Config (SystemCoreClock / 1000);
	
  while(1)
	{
		_sw_delay_ms(1000);
		sw_rtc_gettime( &currTime );
	}
}

 /* SysTick interrupt Handler. */
void SysTick_Handler(void)  
{                              
  msTicks++;                                               
	if (msTicks >= SW_RTC_TICK_PER_SECOND)
	{
		msTicks = 0;
		sw_rtc_update();
	}
}

/* Software delay */
static void _sw_delay_ms( uint32_t period )
{
	uint32_t i = 0;
	uint32_t delay_counter = period * 1000;
	for (i = 0; i < delay_counter; i++)
	{
		__asm("NOP");
	}
}