/*
 * systick.c
 *
 *  Created on: Oct 2, 2025
 *      Author: professor
 */
#include <stm32f446xx.h>
#define SYST_CSR_ENABLE 	(1U<<0) //habilita o contador
#define SYST_CSR_CLKSOURCE 	(1U<<2) //determina se o clock eh externo ou nao
#define SYST_CSR_COUNTFLAG 	(1U << 16)
#define SYST_CLK 			16000000
#define BASE_VALUE 			(SYST_CLK/1000) - 1

void delayMs(uint32_t delay){
	if(delay == 0) return;
	uint32_t reload = BASE_VALUE;
	if(reload > 0XFFFFFFUL) reload = 0XFFFFFFUL;
	SysTick->CTRL = 0U;
	SysTick->LOAD = reload;
	SysTick->VAL = 0U;
	SysTick->CTRL = SYST_CSR_CLKSOURCE | SYST_CSR_ENABLE;

	while(delay--){
		while((SysTick->CTRL &  SYST_CSR_COUNTFLAG) == 0U);

	}

	SysTick->VAL = 0U;
	SysTick->CTRL = 0U;

}


