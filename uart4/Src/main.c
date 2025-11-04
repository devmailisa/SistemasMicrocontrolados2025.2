#include <stdint.h>
#include <stdio.h>
#include "stm32f446xx.h"
#include <uart4.h>
#include <systick.h>
#include <fpu.h>

#define baudRate	115200
#define clkPerif	16000000

int main(void)
{
	uart4RxTxIni(baudRate, clkPerif);
	fpuEnable();

	float valor;

	string inputString = "";
	while(1)
	{
		printf("Informe um valor:\n");
		inputString = uart4ReadString();
		sscanf(inputString, "%f", &valor); //@suppress("Float formatting support")
		printf("Valor informado: %.2f\n\r", valor); //@suppress("Float formatting support")
		delayMs(2000);
	}

	return 0;
}
