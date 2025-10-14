#include <stdint.h>
#include <uart2.h>
#include <systick.h>
#include <stdio.h>
#include <fpu.h>
#define baudRate 	115200
#define clk			16000000

int main(void)
{
	uart2RxTxIni(baudRate,clk);

	while(1)
	{
		uart2SendString("IFPB-Sistemas Microcontrolados\r\n");
		delayMs(1000);
	}
}
