/*
 * uart.h
 * function prptotypes for uart
 */

#ifndef UART_H_
#define UART_H_

// yep
void uartInit(uint32_t);
void uartSendChar(char);
char uartGetChar(void);
void uartPrint(const char * data);
void uartPrintln(const char * data);

#endif
