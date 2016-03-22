#include <avr/io.h>
#include <avr/interrupt.h>
#include "defs.h"
#include "funcs.h"

int main(){
    pinInit();
    interruptInit();
    DDRB |= (1<<PB5);

    while(1){
    }
}

// ISR for reading inputs
volatile uint8_t bits = 0;
ISR(PCINT2_vect){
    // mask off the bits we care about for sensing
    bits = PIND & (SENS1 | SENS2 | SENS3 | SENS4);
}
