#ifndef UART4_H_
#define UART4_H_

void uart4RxTxIni(uint32_t baudRate, uint32_t clkPerif);
void uart4Write(int ch);
int __io_putchar(int ch);
char uart4Read(void);
char* uart4ReadString(void);

#endif /* UART4_H_ */
