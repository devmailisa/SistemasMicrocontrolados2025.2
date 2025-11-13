#include <stdint.h>
#include <adc1.h>
#include <uart2.h>
#include <stdio.h>
#include <stdint.h>
#include <systick.h>
#include <fpu.h>

#define baudRate 	115200
#define clkPerif	16000000

int main(void)
{
	uart2RxTxIni(baudRate, clkPerif);
	adcIni();
	adcStart(18);
	fpuEnable();

	double tensao;
	double temperatura;

	uint16_t leitura;
    while(1)
    {
    	leitura = adcRead();
    	tensao = (leitura*3.3)/(4095.0);
    	temperatura = ((tensao-0.76)/0.025)+25;
    	printf("Valor lido no conversor: %d\n\r",leitura);
    	printf("Valor convertido (tensao): %f\n\r", tensao);
    	printf("Temperatura de microcontrolador: %f\n\r", temperatura);
    	delayMs(1000);
    }
}
