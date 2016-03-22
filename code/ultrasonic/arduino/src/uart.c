/*
 * uart.c
 * UART initialization and related functions
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
#include "defs.h"

void uartInit(uint32_t baud){
    // calculate baud register setting
    uint16_t baudRegVal = (F_CPU / (16 * baud)) - 1;
    UBRR0H = baudRegVal >> 8;
    UBRR0L = baudRegVal;

    // enable receiver and transmitter interrupts, enable receiver and transmitter
    UCSR0B |= (1<<RXCIE0 | 1<<TXCIE0 | 1<<RXEN0 | 1<<TXEN0);
}
