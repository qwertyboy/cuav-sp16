/*
 * main.c
 * Main file, call setup functions, reads lins with ISR
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "defs.h"
#include "funcs.h"
#include "uart.h"

// some variable defs
volatile uint32_t pt1Temp, pt2Temp, pt3Temp, pt4Temp = 0;
volatile uint32_t pt1Time, pt2Time, pt3Time, pt4Time = 0;
uint32_t lastTime = 0;
uint16_t count = 0;
char printBuf[32] = "";

int main(){
    pinInit();
    peripInit();
    uartInit(115200);

    DDRB |= (1<<PB5);
    DDRC |= (1<<PC0 | 1<<PC1 | 1<<PC2 | 1<<PC3);

    while(1){
        uint32_t currTime = micros();
        if(currTime - lastTime > 1000000){
            lastTime = currTime;

            sprintf(printBuf, "%d", (int)pt1Time);
            uartPrint("pt1 time: ");
            uartPrintln(printBuf);
        }
    }
}

// ISR for reading inputs
ISR(PCINT2_vect){
    // mask off the bits we care about for sensing PD2-5
    //bits = ~PIND & (1<<PT1 | 1<<PT2 | 1<<PT3 | 1<<PT4);

    // check pt1
    if(PIND & (1<<PT1)){
        pt1Temp = micros();
    }else{
        pt1Time = micros() - pt1Temp;
    }

    // check pt2
    if(PIND & (1<<PT2)){
        pt2Temp = micros();
    }else{
        pt2Time = micros() - pt2Temp;
    }

    // check pt3
    if(PIND & (1<<PT3)){
        pt3Temp = micros();
    }else{
        pt3Time = micros() - pt3Temp;
    }

    // check pt4
    if(PIND & (1<<PT4)){
        pt4Temp = micros();
    }else{
        pt4Time = micros() - pt4Temp;
    }
}
