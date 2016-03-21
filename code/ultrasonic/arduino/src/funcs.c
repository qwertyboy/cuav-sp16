#include <avr/io.h>
#include <avr/interrupt.h>
#include "defs.h"

void pinInit(void){
  // input pins, pullups
  DDRB &= ~(SENS1);
  DDRC &= ~(SENS2);
  DDRD &= ~(SENS3 | SENS4);
  
  PORTB |= (SENS1);
  PORTC |= (SENS2);
  PORTD |= (SENS3 | SENS4);
}

void interruptInit(void){

}
