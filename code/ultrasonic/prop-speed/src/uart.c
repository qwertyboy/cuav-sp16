/*
 * uart.c
 * UART initialization and related functions
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "defs.h"

// uart initialization
void uartInit(uint32_t baud){
    // calculate baud register setting
    uint16_t baudrate = (F_CPU / (16 * baud)) - 1;
    UBRR0H = baudrate >> 8;
    UBRR0L = baudrate;

    // enable receiver and transmitter interrupts, enable receiver and transmitter
    UCSR0B |= (1<<RXEN0 | 1<<TXEN0);
}

// function to send a character
void uartSendChar(char data){
    // wait until transmit buffer is ready to receive data
    while(!(UCSR0A & (1<<UDRE0)));
    // send!
    UDR0 = data;
}

// blocking(?) function to get data
char uartGetChar(void){
    // wait until data is received, this is probably blocking
    while(!(UCSR0A & (1<<RXC0)));
    return UDR0;
}

// function to send a string
void uartPrint(char * data){
    uint16_t i = 0;
    while(data[i] != '\0'){
        uartSendChar(data[i]);
        i++;
    }
}

// function to send a string with a newline after
void uartPrintln(char * data){
    uartPrint(data);
    uartPrint("\r\n");
}
