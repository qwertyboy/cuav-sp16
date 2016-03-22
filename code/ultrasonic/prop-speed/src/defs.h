/*
 * defs.h
 * Constant definitions
 */

#ifndef DEFS_H_
#define DEFS_H_

// input pin defs
#define SENS1 0x04  // PD2, PCINT18
#define SENS2 0x08  // PD3, PCINT19
#define SENS3 0x10  // PD4, PCINT20
#define SENS4 0x20  // PD5, PCINT21

// nop for short delays
#define _NOP() do {__asm__ __volatile__ ("nop");} while(0)

#endif
