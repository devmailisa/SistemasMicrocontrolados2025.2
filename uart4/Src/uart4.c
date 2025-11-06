/*
 * UART4_TX está em PA0
 * UART4_RX está em PA1
 */
#include "stm32f446xx.h"
#include <stdint.h>

#define enableGPIOA	(1U<<0)
#define UART4EN		(1U<<19)

#define TE			(1U<<3) //Transmissor PA0 -> A0
#define UE			(1U<<13)
#define RE			(1U<<2) //Receptor PA1 -> A1
#define TXE			(1U<<7)
#define RXNE		(1U<<5)

#define BUFFER_SIZE	100

char uart4Buffer[BUFFER_SIZE];
uint32_t uart4Pos = 0;
void uart4RxTxIni(uint32_t baudRate, uint32_t clkPerif)
{
	uint16_t uartDiv;

	//Habilitando o clock da GPIOA
	RCC->AHB1ENR |= enableGPIOA;

	//Configurando PA0 e PA1 como FA
	GPIOA->MODER |= (1U<<1) | (1U<<3);
	GPIOA->MODER &= ~(1U<<0);
	GPIOA->MODER &= ~(1U<<2);

	//Configurando PAO como FA8
	GPIOA->AFR[0] |= (1U<<3);
	GPIOA->AFR[0] &= ~(1U<<2);
	GPIOA->AFR[0] &= ~(1U<<1);
	GPIOA->AFR[0] &= ~(1U<<0);

	//Configurando PA1 como FA8
	GPIOA->AFR[0] |= (1U<<7);
	GPIOA->AFR[0] &= ~(1U<<6);
	GPIOA->AFR[0] &= ~(1U<<5);
	GPIOA->AFR[0] &= ~(1U<<4);

	//Habilitando o clock do reg. UART4EN
	RCC->APB1ENR |= UART4EN;

	//Configurando o uartDiv
	uartDiv = (clkPerif + (baudRate/2))/baudRate;
	UART4->BRR = uartDiv;
	UART4->CR1 |= TE;
	UART4->CR1 |= UE;
	UART4->CR1 |= RE;
}

void uart4Write(int ch)
{
	//Esvaziando o Data Register para habilitá-lo para transmitir
	while(!(UART4->SR & TXE))
	{

	}

	UART4->DR = (ch & 0xFF);
}

//Funcao para habilitar a "transmissao" via printf
int __io_putchar(int ch)
{
	uart4Write(ch);
	return ch;
}

char uart4Read(void)
{
	while(!(UART4->SR & RXNE))
	{

	}
	return (UART4->DR & 0xFF);
}

char* uart4ReadString(void)
{
	char ch;
	uart4Pos = 0;
	ch = uart4Read();

	while(ch != '\n' && uart4Pos < BUFFER_SIZE-1)
	{
		uart4Buffer[uart4Pos++] = ch;
		ch = uart4Read();
	}

	uart4Buffer[uart4Pos] = '\0';

	return uart4Buffer;
}


