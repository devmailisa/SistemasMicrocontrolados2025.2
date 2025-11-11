#include <stdint.h>
#include <adc1.h>
#include <uart2.h>
#include <stdio.h>
#include <stdint.h>

#define baudRate 	115200
#define clkPerif	16000000

int main(void)
{
	uart2RxTxIni(baudRate, clkPerif);
	adcIni();
	adcStart(1);
	uint16_t leitura;
    while(1)
    {
    	leitura = adcRead();
    	printf("Valor lido: %d\n\r",leitura);
    	delayMs(1000);
    }
}
