#ifndef DEFS_H_
#define DEFS_H_

// nop for short delays
#define _NOP() do {__asm__ __volatile__ ("nop");} while(0)

// input pin defs
#define SENS1 0x00  // PB0, PCINT0
#define SENS2 0x00  // PC0, PCINT8
#define SENS3 0x00  // PD0, PCINT16
#define SENS4 0x04  // PD2, INT0 or PCINT18

#endif
