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
    DDRD &= ~(1<<PT1 | 1<<PT2 | 1<<PT3 | 1<<PT4);
    //PORTD |= (1<<PT1 | 1<<PT2 | 1<<PT3 | 1<<PT4);
}

// interrupt initializations
void peripInit(void){
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

    /*
    // this is to enable 31.25kHz pwm on timer/counter 1
    // timer/counter1 control register A
    // 8 bit phase correct, set COM1A1
    TCCR1A |= 0x81;
    // clkio / 1 - no prescaling (32kHz?)
    TCCR1B |= 0x02;
    */

    sei();
}

// timekeeping function yanked from arduino
uint32_t micros(){
	uint32_t m;
	uint8_t t;

	cli();

	m = timer0_overflow_count;
	t = TCNT0;

    // This comment is self explanatory.
	if((TIFR0 & 1) && (t < 255)){
		m++;
	}

	sei();

	return ((m << 8) + t) * 4;
}
