/*
 * debounce.c
 *
 *  Created on: Aug 30, 2021
 *      Author: Lenovo
 */

#include "debounce.h"

#include "stm32f4xx_ll_exti.h"

#include "main.h"

#define REF_DEBOUNCE_PERIOD		  200U	/*Debounce time, 10 == 10mS  1000 == 1Sec, must be mutiple of 1mS*/
#define REF_DEBOUNCE_TIMEOUT_CNT  1000UL /* x 1ms = 1 second */

static uint8_t StatoIn1 = 3;

static uint8_t In1 = 3;
static uint8_t In1_0 = 0;
static uint8_t In1_1 = 0;

extern uint8_t volatile __debouce_btn_en;
volatile uint16_t       __debounce_timeout = 0;

void _debounce_restart (void)
{
	StatoIn1 = 3;
	__debouce_btn_en = 0; /* Stop checking debouncing called in systick timer*/
	/* Re-enable exti interrupt */
	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_0);
}

void debounce_handler (void)
{
	In1 = LL_GPIO_IsInputPinSet(BTN_GPIO_Port, BTN_Pin);

	if (In1 == 0)
	{
		In1_0++;
		In1_1 = 0;
		if (In1_0 >= REF_DEBOUNCE_PERIOD)
		{
			In1_0 = REF_DEBOUNCE_PERIOD + 1;
			StatoIn1 = 0;
			_debounce_restart();
		}
	}
	else
	{
		In1_0 = 0;
		In1_1++;
		if (In1_1 >= REF_DEBOUNCE_PERIOD)
		{
			In1_1 = REF_DEBOUNCE_PERIOD + 1;
			StatoIn1 = 1;
//			LL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
			_debounce_restart();
		}
	}

	/* Handle timeout */
	if ( __debounce_timeout++ > REF_DEBOUNCE_TIMEOUT_CNT )
	{
		__debounce_timeout = 0;
		_debounce_restart();
	}
}

uint8_t debouce_get_btn_state (void)
{
	return (StatoIn1 == 1);
}

void  debouce_run_test (void)
{
	if (debouce_get_btn_state() == 1)
	{
//		LL_GPIO_TogglePin(LD4_GPIO_Port, LD4_Pin);
	}
}
