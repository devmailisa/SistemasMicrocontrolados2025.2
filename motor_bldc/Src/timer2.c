#include <stdint.h>

#include "stm32f446xx.h"
#include <systick.h>
#define enableGPIOA (1U<<0)
#define TIM2EN		(1U<<0)


void setDuty(uint8_t canal, uint32_t duty){
	switch (canal){
	case 1:
		TIM2->CCR1=duty;
		break;
	default:
		break;
	}
}


//CONFIGURANDO O TIMER2 PARA GERAR PWM PELO METODO CENTER ALIGNED
//O PULSO DE ONDA QUADRADO TEM 50HZ E PRESCALER DE 160 - 1 PARA UMA FREQUENCIA DE CLOCK INTERNA DE 16MHz
void configurarTimer2(){
	RCC->AHB1ENR |= enableGPIOA; //Habilitando o clock do GPIOA

	GPIOA->MODER &= ~(1U<<0);
	GPIOA->MODER |= (1U<<1);

	//3 2 1 0
	GPIOA->AFR[0] |= (1U<<0);
	GPIOA->AFR[0] &= ~(1U<<1);
	GPIOA->AFR[0] &= ~(1U<<2);
	GPIOA->AFR[0] &= ~(1U<<3);

	RCC->APB1ENR |= (1U<<0); //Habilitando o clock do Timer 2
	TIM2->PSC = 160 - 1;
	TIM2->ARR = 1000;
	TIM2->CNT = 0;

	//CH1 - NÃO INVERTIDO AUMENTA EM NÍVEL ALTO
	TIM2->CCMR1 |= (1U<<6);
	TIM2->CCMR1 |= (1U<<5);
	TIM2->CCMR1 &= ~(1U<<4);

	TIM2->CCER |= (1U<<0);

	//Habilita o timer
	TIM2->CR1 |= (1U<<0);

	setDuty(1, 0);
}
