/*
 * uart2.h
 *
 *  Created on: Oct 7, 2025
 *      Author: 20221610005
 */

#ifndef UART2_H_
#define UART2_H_

void uart2RxTxIni(uint32_t baudRate, uint32_t clkPerif);
void uart2Write(int ch);
char uart2Read(void);
char* uart2ReadString(void);
void uart2SendString(const char *str);

#endif /* UART2_H_ */
