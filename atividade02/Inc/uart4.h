/*
 * uart2.h
 *
 *  Created on: Oct 7, 2025
 *      Author: 20221610005
 */

#ifndef UART4_H_
#define UART4_H_

void uart4RxTxIni(uint32_t baudRate, uint32_t clkPerif);
void uart4Write(int ch);
char uart4Read(void);
char* uart4ReadString(void);
void uart4SendString(const char *str);

#endif /* UART4_H_ */
