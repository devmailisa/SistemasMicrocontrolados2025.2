#include "stm32f446xx.h"

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <systick.h>
#include <fpu.h>
#include <uart4.h>

#define baudRate 	115200
#define clk			16000000

#define GPIOAEN 	(1U<<0)

#define LED2 (1U<<5)

/*
 * GPIOx_IDR é o registrador de leitura -> coloca dentro da estrutura de
 * GPIOx_ODR é o registrador que habilita e desabilita
 *
 * */

int main(void)
{
	//Habilitando o clock da GPIOA
	RCC->AHB1ENR |= GPIOAEN;


	//Configurando PA5 como output
	GPIOA->MODER |= (1U << 10);
	GPIOA->MODER &= ~(1U << 11);

	uart4RxTxIni(baudRate,clk);

	while(1)
	{
		GPIOA->ODR &= ~LED2;
		uart4SendString("Informe a frase: ");
		char * texto = uart4ReadString();
		char * texto_invertido = texto;
		int numero_vogais = 0;

		for(int i = strlen(texto)-1; i > -1; i--){
			texto_invertido[strlen(texto) - i] = texto[i];
			if(texto[i]=='a' || texto[i]=='e' || texto[i]=='i' || texto[i]=='o' || texto[i]=='u' || texto[i]=='A' || texto[i]=='E' || texto[i]=='I' || texto[i]=='O' || texto[i]=='U'){
				numero_vogais++;
			}
		}

		strcat(texto_invertido, "\r\n");

		while(numero_vogais--){
			GPIOA->ODR |= LED2;
			delayMs(500);
			GPIOA->ODR &= ~LED2;
			delayMs(500);
		}

	}
}
