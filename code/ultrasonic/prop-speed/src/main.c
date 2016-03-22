#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "defs.h"
#include "funcs.h"
#include "uart.h"

// some variable defs
uint32_t lastTime = 0;
uint16_t count = 0;
char printBuf[32] = "";

int main(){
    pinInit();
    interruptInit();
    uartInit(115200);

    DDRB |= (1<<PB5);
    DDRC |= (1<<PC0 | 1<<PC1 | 1<<PC2 | 1<<PC3);

    while(1){
        uint32_t currTime = micros();
        if(currTime - lastTime > 1000000){
            lastTime = currTime;

            sprintf(printBuf, "%d", count++);
            uartPrint("fucking words! ");
            uartPrintln(printBuf);
        }


    }
}

// ISR for reading inputs
volatile uint8_t bits = 0;
ISR(PCINT2_vect){
    // mask off the bits we care about for sensing
    bits = PIND & (SENS1 | SENS2 | SENS3 | SENS4);
}
