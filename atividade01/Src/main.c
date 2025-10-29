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

#define LED1 (1U<<4)
#define LED2 (1U<<5)
#define LED3 (1U<<6)
#define LED4 (1U<<7)

/*
 * GPIOx_IDR é o registrador de leitura -> coloca dentro da estrutura de
 * GPIOx_ODR é o registrador que habilita e desabilita
 *
 * */

void status()
{
	bool statusLed1 = (GPIOA->IDR & LED1) != 0;
	bool statusLed2 = (GPIOA->IDR & LED2) != 0;
	bool statusLed3 = (GPIOA->IDR & LED3) != 0;
	bool statusLed4 = (GPIOA->IDR & LED4) != 0;

	statusLed1 ? uart4SendString("L1=ON,"): uart4SendString("L1=OFF,");
	statusLed2 ? uart4SendString("L2=ON,") : uart4SendString("L2=OFF,");
	statusLed3 ? uart4SendString("L3=ON,") : uart4SendString("L3=OFF,");
	statusLed4 ? uart4SendString("L4=ON\r\n") : uart4SendString("L4=OFF\r\n");
}

void on(int n){
	switch(n){
	case 1:
		GPIOA->ODR |= LED1;
		uart4SendString("LED 01 ON\r\n");
		break;
	case 2:
		GPIOA->ODR |= LED2;
		uart4SendString("LED 02 ON\r\n");
		break;
	case 3:
		GPIOA->ODR |= LED3;
		uart4SendString("LED 03 ON\r\n");
		break;
	case 4:
		GPIOA->ODR |= LED4;
		uart4SendString("LED 04 ON\r\n");
		break;
	default:
		uart4SendString("ERR\r\n");
	}
}

void off(int n){
	switch(n){
	case 1:
		GPIOA->ODR &= ~LED1;
		uart4SendString("LED 01 OFF\r\n");
		break;
	case 2:
		GPIOA->ODR &= ~LED2;
		uart4SendString("LED 02 OFF\r\n");
		break;
	case 3:
		GPIOA->ODR &= ~LED3;
		uart4SendString("LED 03 OFF\r\n");
		break;
	case 4:
		GPIOA->ODR &= ~LED4;
		uart4SendString("LED 04 OFF\r\n");
		break;
	default:
		uart4SendString("ERR\r\n");
	}
}

void toggle(int n){
	switch(n){
	case 1:
		GPIOA->ODR ^= LED1;
		uart4SendString("LED 01 TOGGLED\r\n");
		break;
	case 2:
		GPIOA->ODR ^= LED2;
		uart4SendString("LED 02 TOGGLED\r\n");
		break;
	case 3:
		GPIOA->ODR ^= LED3;
		uart4SendString("LED 03 TOGGLED\r\n");
		break;
	case 4:
		GPIOA->ODR ^= LED4;
		uart4SendString("LED 04 TOGGLED\r\n");
		break;
	default:
		uart4SendString("ERR\r\n");
	}
}

int main(void)
{
	//Habilitando o clock da GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	//Configurando PA4 como output
	GPIOA->MODER |= (1U << 8);
	GPIOA->MODER &= ~(1U << 9);

	//Configurando PA5 como output
	GPIOA->MODER |= (1U << 10);
	GPIOA->MODER &= ~(1U << 11);

	//Configurando PA6 como output
	GPIOA->MODER |= (1U << 12);
	GPIOA->MODER &= ~(1U << 13);

	//Configurando PA7 como output
	GPIOA->MODER |= (1U << 14);
	GPIOA->MODER &= ~(1U << 15);

	uart4RxTxIni(baudRate,clk);

	while(1)
	{
		printf("Olá, o que você gostaria fazer agora?");
		char* ans = uart4ReadString();

		if(strcmp(ans, "STATUS")) status();
		if(strcmp(ans, "ON 1")) on(1);
		if(strcmp(ans, "ON 2")) on(2);
		if(strcmp(ans, "ON 3")) on(3);
		if(strcmp(ans, "ON 4")) on(4);
		if(strcmp(ans, "OFF 1")) off(1);
		if(strcmp(ans, "OFF 2")) off(2);
		if(strcmp(ans, "OFF 3")) off(3);
		if(strcmp(ans, "OFF 4")) off(4);
		if(strcmp(ans, "TOGGLE 1")) toggle(1);
		if(strcmp(ans, "TOGGLE 2")) toggle(2);
		if(strcmp(ans, "TOGGLE 3")) toggle(3);
		if(strcmp(ans, "TOGGLE 4")) toggle(4);

	}
}
