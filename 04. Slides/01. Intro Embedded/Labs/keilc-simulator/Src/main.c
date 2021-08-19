#include "ARMCM4.h"

typedef unsigned int uint32_t;

volatile uint32_t msTicks = 0;                              /* Variable to store millisecond ticks */
  
extern uint32_t SystemCoreClock;

void SysTick_Handler(void)  {                               /* SysTick interrupt Handler. */
  msTicks++;                                               
}


int main (void)  {
  uint32_t returnCode;
  
 SysTick_Config(SystemCoreClock / 1000);      /* Configure SysTick to generate an interrupt every millisecond */
 
  while(1);
}