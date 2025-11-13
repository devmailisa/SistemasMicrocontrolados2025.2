#include "stm32f446xx.h"

void fpuEnable(void)
{
	SCB->CPACR |= (1U<<20) | (1U<<21) | (1U<<22) | (1U<<23);
}

