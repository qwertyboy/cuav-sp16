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
  // pin change mask register, enable interrupts on corresponding pins
  PCMSK0 |= (1<<PCINT0);
  PCMSK1 |= (1<<PCINT8);
  PCMSK2 |= (1<<PCINT16);
  // pin change interrupt control register, enable PCIE0-2
  PCICR |= (1<<PCIE0 | 1<<PCIE1 | 1<<PCIE2);

  // external inperrupt mask register, enable interrupt on corresponding pins
  EIMSK |= (1<<INT0);
  // external interrupt control register, generate interrupt on change
  EICRA |= (1<<ISC00);
}
