#include "includes/processor.h"

__attribute__((weak))
void pReset(cpu *p) {
    p->a = 0;
    p->x = 0;
    p->y = 0;
    p->sp = 0xFF;
    for (int i = 0; i < 0xFFFF; i++) p->mem[i] = 0;
    p->pc = (p->mem[0xFFD] << 8) | p->mem[0xFFC];
}

__attribute__((weak))
byte pRead(cpu *p, byte *addr) {
    sleep(CLKTIME);
    return (*addr);
}

__attribute__((weak))
word pReadAddr(cpu *p, word offset) {
    word val = (pRead(p, p->mem + offset + 1) << 8);
    val |= pRead(p, p->mem+offset);
    return val;
}

__attribute__((weak))
void pWrite(cpu *p, byte *addr, byte val) {
    sleep(CLKTIME);
    *addr = val;
    return;
}

__attribute__((weak))
byte pReadCntr(cpu *p) {
    return p->mem[p->pc++];
}

__attribute__((weak))
void pSetCntr(cpu *p, word val) {
    sleep(CLKTIME*2);
    p->pc = val;
    return;
}