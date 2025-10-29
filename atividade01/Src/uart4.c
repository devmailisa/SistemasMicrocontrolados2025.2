#include <stm32f446xx.h>
#include <stdint.h>
#include <string.h>

#define enableGPIOA (1U<<0) //habilita o clock da GPIO A
#define UART4EN 	(1U<<19)//habilita o clock da UART4
#define UE 			(1U<<13)
#define TE			(1U<<3)
#define TXE 		(1U<<7)
#define RE			(1U<<2)
#define RXNE		(1U<<5)
#define BUFFER_SIZE	100

char uart4Buffer[BUFFER_SIZE];
uint32_t uart4Pos;

void uart4Write(int ch)
{
	while(!(UART4->SR & TXE))
	{

	}

	//DR -> registrador de dados
	UART4->DR = (ch & 0xFF);
}

int __io_putchar(int ch)
{
	uart4Write(ch);
	return ch;
}

void uart4RxTxIni(uint32_t baudRate, uint32_t clkPerif)
{
	uint16_t uartDiv;
	RCC->AHB1ENR |= enableGPIOA;

	RCC->APB1ENR |= UART4EN;

	//Habilitando o modo de operacao do PA0 como FA-> 1 0
	GPIOA->MODER |= (1U<<1);
	GPIOA->MODER &= ~(1U<<0);

	//O AFRL é o conjunto de registradores que trabalham como função alternativa
	//Habilitando a funcao alternativa PA0 - TX
	GPIOA->AFR[0] &= ~(1U<<2) & ~(1U<<1) & ~(1U<<0);
	GPIOA->AFR[0] |= (1U<<3);

	//Habilitando o modo de operacao de PA1 como FA -> 3 2
	GPIOA->MODER |= (1U<<3);
	GPIOA->MODER &= ~(1U<<2);

	//Habilitando a funcao alternativa PA1 - RX 1000
	GPIOA->AFR[0] &= ~(1U<<6) & ~(1U<<5) & ~(1U<<4); //6 5 4
	GPIOA->AFR[0] |= (1U<<7); //7

	//Calculo do valor da uartDiv para impor o baud
	//Registrador de controle da USART
	uartDiv = (clkPerif + (baudRate/2))/baudRate;
	USART2->BRR = uartDiv;
	USART2->CR1 |= TE;
	USART2->CR1 |= UE;
	USART2->CR1 |= RE;

}

char uart4Read(void)
{
	while(!(UART4->SR & RXNE))
	{

	}

	return (UART4->DR & 0xFF);
}

//Funciona como scanf
char* uart4ReadString(void)
{
	char ch;
	uart4Pos = 0;
	ch = uart4Read();

	//Enquanto não for encontrado o caractere de nova linha e não tiver estouro de memória
	while(ch != '\n' && uart4Pos < BUFFER_SIZE-1)
	{
		uart4Buffer[uart4Pos++] = ch;
		ch = uart4Read();
	}

	uart4Buffer[uart4Pos] = '\0';

	return uart4Buffer;
}

void uart4SendString(const char *str)
{
	while(*str != '\0')
	{
		uart4Write(*str);
		str++;
	}
}

