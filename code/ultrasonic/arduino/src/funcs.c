/*
 * funcs.c
 * Setup and helper functions
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "funcs.h"
#include "defs.h"

// timer 0 overflow interrupt for timing
volatile uint32_t timer0_overflow_count = 0;
ISR(TIMER0_OVF_vect){
	timer0_overflow_count++;
}

// I/O pin initializations
void pinInit(void){
    // input pins, pullups
    DDRD &= ~(SENS1 | SENS2 | SENS3 | SENS4);
    PORTD |= (SENS1 | SENS2 | SENS3 | SENS4);
}

// interrupt initializations
void interruptInit(void){
    cli();
    // pin change mask register, enable interrupts on corresponding pins
    PCMSK2 |= (1<<PCINT18 | 1<<PCINT19 | 1<<PCINT20 | 1<<PCINT21);
    // pin change interrupt control register, enable PCIE2
    PCICR |= (1<<PCIE2);

    // timer/counter0 control register A, fast pwm
	TCCR0A |= 0x03;
	// clkio / 64
	TCCR0B |= 0x03;
	// enable overflow interrupt
	TIMSK0 |= 0x01;

    sei();
}

// timekeeping function yanked from arduino
uint32_t micros(){
	uint32_t m;
	uint8_t t;

	cli();

	m = timer0_overflow_count;
	t = TCNT0;

	if((TIFR0 & 1) && (t < 255)){
		m++;
	}

	sei();

	return ((m << 8) + t) * 4;
}
