/*
 * CONTROLADOR DO MOTOR BLDC COM MCU STM32F446RExx
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include <systick.h>
#include <fpu.h>
#include <uart2.h>
#include <timer2.h>

#define baudRate 	115200
#define clk			16000000

#define dCMin 53
#define dCMax 118


//Gerando pulso de throller a partir do valor de entrada na uart2
int thr(uint32_t en, uint32_t *dcAtual)
{
    if(en < 0 || en > 100) return 0;

    uint32_t dc;
    float passo;
    passo = (dCMax - dCMin)/99.0;

    //printf("passo: %f\r\n", passo);
    dc = dCMin + passo*(en - 1); //COMPARADOR
    //printf("dc: %d\r\n", dc);

    if(*dcAtual > dc)
    {
        while(*dcAtual != dc)
        {
            uint32_t diff = *dcAtual - dc;
            if(diff >= 2)
            {
                *dcAtual -= 2;
            } else
            {
                *dcAtual -= diff;
            }
        }

    } else if(*dcAtual < dc)
    {
        while(*dcAtual != dc)
        {
            uint32_t diff = dc - *dcAtual;
            if(diff >= 2)
            {
                *dcAtual += 2;
            } else
            {
                *dcAtual += diff;
            }
        }
    }

    //printf("dcAtual: %d\r\n", *dcAtual);

    return 1;
}


int check_in(char * entrada, char * func, uint8_t * val) {

    int final_func = strnlen(func, 20);

    if (strncmp(entrada, func, final_func) != 0) {
        return 0;
    }

    int digitos = 0;
    int i = final_func;
    char num[3];
    while (isdigit(entrada[i])) {
        num[digitos] = entrada[i];
        digitos++;
        i++;
        if (digitos > 3) {
            return 0;
        }
    }

    if (entrada[i] != '\0') {
        return 0;
    }

    *val = atoi(num);
    //printf("%d\r\n", *val);
    return 1;
}

int main(void)
{
	configurarTimer2();
	fpuEnable();
	uart2RxTxIni(baudRate, clk);

	char * entrada;
	uint8_t val;
	uint32_t dcAtual = 53;

	while(1)
	{
		entrada = uart2ReadString();

		if(check_in(entrada, "THR ", &val))
		{
			//chama THR
			int err_thr = thr(val, &dcAtual);
			if(!err_thr)
			{
				printf("ERR:ALVO DO THROLLER FORA DO INTERVALO DE [0, 100]\r\n");
			} else
			{
				printf("THROLLER AJUSTADO COM SUCESSO\r\n");
			}
		} else if (check_in(entrada, "SET STEP=", &val))
		{
			//chama SET STEP
		} else if (check_in(entrada, "SET HOLD=", &val))
		{
			//chama SET HOLD
		}
	}
}
