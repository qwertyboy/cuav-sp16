/*
 * defs.h
 * Constant definitions
 */

#ifndef DEFS_H_
#define DEFS_H_

// input pin defs
#define PT1 2 // PD2, PCINT18
#define PT2 3 // PD3, PCINT19
#define PT3 4 // PD4, PCINT20
#define PT4 5 // PD5, PCINT21

// nop for short delays
#define _NOP() do {__asm__ __volatile__ ("nop");} while(0)

#endif
