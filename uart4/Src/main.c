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

	char * inputString = "";
	while(1)
	{
		printf("Informe um valor:\r\n");
		inputString = uart4ReadString();
		printf("Valor informado: %s\r\n", inputString);
		delayMs(2000);
	}

	return 0;
}
