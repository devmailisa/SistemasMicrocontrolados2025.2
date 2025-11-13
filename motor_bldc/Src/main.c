#include <stdint.h>
#include <systick.h>
#include <stdio.h>
#include <fpu.h>
#include <uart2.h>
#include <timer2.h>
#define baudRate 	115200
#define clk			16000000

#define dCMin 53
#define dCMax 118


//Gerando pulso de throller a partir do valor de entrada na uart2
void thr(uint32_t en, double dCAtual)
{
	uint8_t dc;
	double passo = (dCMax - dCMin)/100;
	dc = 53 + passo*en;

	//duty cycle de 53 a 118
	while(1)
	{
		//Configurar a rampa aqui
	}
}


int main(void)
{
	configurarTimer2();
	fpuEnable();
	uart2RxTxIni(baudRate, clk);

	while(1)
	{
		//O SINAL ESTÁ SAINDO NA PA0 QUE JÁ ESTÁ CONFIGURADA
		//A UART RECEBE PELO PA2 E ENVIA PELO PA3

	}
}
