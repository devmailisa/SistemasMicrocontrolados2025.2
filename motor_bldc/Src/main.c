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
int thr(uint32_t en, uint32_t dcAtual)
{
	if(en < 1 || en > 100) return 0;

	uint32_t dc;
	float passo;

	passo = (dCMax - dCMin)/99;
	dc = dCMin + passo*(en - 1); //COMPARADOR

	if(dcAtual > dc)
	{
		while(dcAtual != dc)
		{
			uint32_t diff = dcAtual - dc;
			if(diff >= 2)
			{
				dcAtual -= 2;
			} else
			{
				dcAtual -= diff;
			}

			setDuty(1, dcAtual);
			delayMs(50);
		}

	} else if(dCAtual < dc)
	{
		while(dcAtual != dc)
		{
			uint32_t diff = dc - dcAtual;
			if(diff >= 2)
			{
				dcAtual += 2;
			} else
			{
				dcAtual += diff;
			}

			setDuty(1, dcAtual);
			delayMs(50);
		}
	}

	return dcAtual;
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
