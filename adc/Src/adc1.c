#include "stm32f446xx.h"
#include <stdio.h>
#include <systick.h>

void adcIni(void)
{
	RCC->AHB1ENR |= (1U<<0);
	GPIOA->MODER |= (1U << 2);
	GPIOA->MODER |= (1U<<3);

	RCC->APB2ENR |= (1U << 8);

	ADC->CCR &= ~(1U << 16);
	ADC->CCR &= ~(1U << 17);
	ADC1->CR1 &= ~(1U << 24); //CONFIGURANDO RESOLUÇÃO
	ADC1->CR1 &= ~(1U << 25); //CONFIGURANDO RESOLUÇÃO

	ADC1->SQR1 &= ~(1U << 20); //QUANTIDADE DE CANAIS LIDOS
	ADC1->SQR1 &= ~(1U << 21);
	ADC1->SQR1 &= ~(1U << 22);
	ADC1->SQR1 &= ~(1U << 23);

	ADC1->CR2 |= (1U << 0);
	delayUs(2);
}

void adcStart(int canal) //Inicia a conversao no canal especifico
{
	ADC1->SQR3 = 0;
	ADC1->SQR3 |= (canal << 0);
	ADC1->SR = 0; //Limpeza do registrador de estados
	ADC1->CR2 |= (1U<<30); //inicia a conversao
}

uint16_t adcRead(void)
{
	while(!(ADC1->SR & (1U<<1)))
	{

	}

	return ADC1->DR;
}
