#pragma once
#include <stdint.h>
#include <unistd.h>

typedef uint8_t byte;
typedef uint16_t word;

#define sleep(x) usleep(x * 1000);
#define CLKTIME 1

typedef struct processor {
    byte x, y, a, sp;
    word pc;
    byte flags;
    byte mem[0x10000];
} cpu;

__attribute__((weak))
void pReset(cpu *p);

__attribute__((weak))
byte pRead(cpu *p, byte *addr);
__attribute__((weak))
word pReadAddr(cpu *p, word offset);
__attribute__((weak))
void pWrite(cpu *p, byte *addr, byte val);

__attribute__((weak))
byte pReadCntr(cpu *p);
__attribute__((weak))
void pSetCntr(cpu *p, word val);