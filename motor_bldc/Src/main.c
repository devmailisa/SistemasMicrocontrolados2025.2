/*
 * CONTROLADOR DO MOTOR BLDC COM MCU STM32F446REX
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

//FUNÇÃO PARA ALTERAR A AMPLITUDADE DA RAMPA
int set_step(uint32_t * step, uint32_t new_step)
{
	if(new_step > 0 && new_step < 11){
		*step = new_step;
		return 1;
	}

	return 0;
}

//FUNÇÃO PARA ALTERAR O DELAY ENTRE O ENVIO DO SINAL
void set_hold(uint32_t * delay, uint8_t new_delay)
{
	*delay = new_delay;
}

//GERANDO PULSO DE THROTTLE - ACELERADOR
int thr(uint32_t en, uint32_t *dcAtual, uint32_t * delay, uint32_t * step)
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
            if(diff >= *step)
            {
                *dcAtual -= *step;
            } else
            {
                *dcAtual -= diff;
            }

            delayMs(*delay);
        }

    } else if(*dcAtual < dc)
    {
        while(*dcAtual != dc)
        {
            uint32_t diff = dc - *dcAtual;
            if(diff >= *step)
            {
                *dcAtual += *step;
            } else
            {
                *dcAtual += diff;
            }

            delayMs(*delay);
        }
    }

    //printf("dcAtual: %d\r\n", *dcAtual);

    return 1;
}

//FUNÇÃO PARA VALIDAR A ENTRADA
int check_in(char *entrada, char *func, uint32_t *val) {
    int final_func = strlen(func);

    if (strncmp(entrada, func, final_func) != 0) {
        return 0;
    }

    int digitos = 0;
    int i = final_func;
    char num[4];  // Para armazenar até 3 dígitos + '\0'
    while (isdigit(entrada[i])) {
        num[digitos] = entrada[i];
        digitos++;
        i++;

        // Garantir que não ultrapasse 10 dígitos
        if (digitos > 10) {
            return 0;
        }
    }

    if (digitos == 0) return 0;  // Se não foi lido nenhum dígito

    num[digitos] = '\0';  // Garantir que a string seja corretamente terminada

    if (entrada[i] != '\0') {  // Verifica se há caracteres extras após o número
        return 0;
    }

    // Converte a string num para uint32_t com verificação
    long int temp_val = strtol(num, NULL, 10);
    if (temp_val < 0 || temp_val > UINT32_MAX) {
        return 0;  // Valor fora do intervalo de uint32_t
    }
    *val = (uint32_t)temp_val;

    return 1;
}


int main(void)
{
	configurarTimer2();
	fpuEnable();
	uart2RxTxIni(baudRate, clk);

	char * entrada;
	uint32_t val;
	uint32_t dcAtual = 53;
	uint32_t delay = 50;
	uint32_t step = 2;

	while(1)
	{
		entrada = uart2ReadString();

		if(check_in(entrada, "THR ", &val))
		{
			//chama THR
			int err_thr = thr(val, &dcAtual, &delay, &step);
			if(!err_thr)
			{
				printf("ERR:ALVO DO THROTTLE FORA DO INTERVALO DE [0, 100]\r\n");
			} else
			{
				printf("THROTTLE AJUSTADO COM SUCESSO\r\n");
			}
		} else if (check_in(entrada, "SET STEP=", &val))
		{
			//chama SET STEP
			if(set_step(&step, val))
			{
				printf("STEP AJUSTADO COM SUCESSO\r\n");
			} else
			{
				printf("STEP FORA DO INTERVALO [1, 10]\r\n");
			}

		} else if (check_in(entrada, "SET HOLD=", &val))
		{
			set_hold(&delay, val);
			printf("HOLD AJUSTADO COM SUCESSO\r\n");
		}
	}
}
