#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "defs.h"
#include "funcs.h"
#include "uart.h"

// some variable defs
volatile uint8_t bits = 0;
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
        switch(bits){
            case 1<<PT1:
                PORTC = 0x08;
                break;
            case 1<<PT2:
                PORTC = 0x04;
                break;
            case 1<<PT3:
                PORTC = 0x02;
                break;
            case 1<<PT4:
                PORTC = 0x01;
                break;
            default:
                PORTC = 0x00;
                break;
        }

        uint32_t currTime = micros();
        if(currTime - lastTime > 1000000){
            lastTime = currTime;

            sprintf(printBuf, "%d", count++);
            uartPrint("\rcount: ");
            uartPrint(printBuf);
        }
    }
}

// ISR for reading inputs
ISR(PCINT2_vect){
    // mask off the bits we care about for sensing PD2-5
    bits = ~PIND & (1<<PT1 | 1<<PT2 | 1<<PT3 | 1<<PT4);
}
