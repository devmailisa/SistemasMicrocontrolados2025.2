#include <stm32f446xx.h>
#include <stdint.h>
#include <string.h>

#define enableGPIOA (1U<<0)
#define USART2EN 	(1U<<17)
#define UE 			(1U<<13)
#define TE			(1U<<3)
#define TXE 		(1U<<7)
#define RE			(1U<<2)
#define RXNE		(1U<<5)
#define BUFFER_SIZE	100

char uart2Buffer[BUFFER_SIZE];
uint32_t uart2Pos;

void uart2Write(int ch)
{
	while(!(USART2->SR & TXE))
	{

	}

	//DR -> registrador de dados
	USART2->DR = (ch & 0xFF);
}

int __io_putchar(int ch)
{
	uart2Write(ch);
	return ch;
}

void uart2RxTxIni(uint32_t baudRate, uint32_t clkPerif)
{
	uint16_t uartDiv;
	RCC->AHB1ENR |= enableGPIOA;
	RCC->APB1ENR |= USART2EN;
	GPIOA->MODER |= (1U<<5);
	GPIOA->MODER &= ~(1U<<4);
	GPIOA->AFR[0] |= (1U<<8) | (1U<<9) | (1U<<10);
	GPIOA->AFR[0] &= ~(1U<<11);

	GPIOA->MODER |= (1U<<7);						//PA3 como Funcao Alternativa
	GPIOA->MODER &= ~(1U<<6);						//PA3 como Funcao Alternativa
	GPIOA->AFR[0] |= (1U<<14) | (1U<<13) | (1U<<12);//AF7
	GPIOA->AFR[0] &= ~(1U<<15);						//AF7

	//Calculo do valor da uartDiv para impor o baud
	//Registrador de controle da USART
	uartDiv = (clkPerif + (baudRate/2))/baudRate;
	USART2->BRR = uartDiv;
	USART2->CR1 |= TE;
	USART2->CR1 |= UE;
	USART2->CR1 |= RE;

}

char uart2Read(void)
{
	while(!(USART2->SR & RXNE))
	{

	}

	return (USART2->DR & 0xFF);
}

//Funciona como scanf
char* uart2ReadString(void)
{
	char ch;
	uart2Pos = 0;
	ch = uart2Read();

	//Enquanto não for encontrado o caractere de nova linha e não tiver estouro de memória
	while(ch != '\n' && uart2Pos < BUFFER_SIZE-1)
	{
		uart2Buffer[uart2Pos++] = ch;
		ch = uart2Read();
	}

	uart2Buffer[uart2Pos] = '\0';

	return uart2Buffer;
}

void uart2SendString(const char *str)
{
	while(*str != '\0')
	{
		uart2Write(*str);
		str++;
	}
}

