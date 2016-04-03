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
volatile uint8_t risePt1, risePt2, risePt3, risePt4 = 0;
float rpm1, rpm2, rpm3, rpm4 = 0.0;
uint32_t lastTime = 0;
uint16_t count = 0;
char printBuf[32] = "";

int main(){
    DDRB |= (1<<PB5);
    pinInit();
    peripInit();
    uartInit(115200);

    while(1){
        uint32_t currTime = micros();

        rpm1 = 1 / ((float)pt1Time / 1000000);
        rpm2 = 60 / pt2Time;
        rpm3 = 60 / pt3Time;
        rpm4 = 60 / pt4Time;

        if(currTime - lastTime > 1000000){
            lastTime = currTime;

            sprintf(printBuf, "%d", count++);
            uartPrint("\nReading number: ");
            uartPrintln(printBuf);

            sprintf(printBuf, "%d", pt1Time);
            uartPrint("RPM1: ");
            uartPrintln(printBuf);
            /*
            sprintf(printBuf, "%d", rpm2);
            uartPrint("RPM2: ");
            uartPrintln(printBuf);

            sprintf(printBuf, "%d", rpm3);
            uartPrint("RPM3: ");
            uartPrintln(printBuf);

            sprintf(printBuf, "%d", rpm4);
            uartPrint("RPM4: ");
            uartPrintln(printBuf);
            */
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
        risePt1 ^= 1;
    }else if(risePt1 == 0){
        pt1Time = micros() - pt1Temp;
        PORTB ^= (1<<PB5);
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
