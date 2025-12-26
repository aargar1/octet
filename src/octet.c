#include "includes/octet.h"

word IMM(cpu *p) {
    return p->pc++; 
}

word ZP(cpu *p) {
    return (word)pReadCntr(p);
}
word ZPX(cpu *p) {
    return (word)((pReadCntr(p) + p->x) & 0xFF);
}

word ZPY(cpu *p) {
    return (word)((pReadCntr(p) + p->y) & 0xFF);
}

word ABS(cpu *p) {
    word lo = p->mem[p->pc];
    word hi = p->mem[p->pc + 1];
    word addr = (hi << 8) | lo;
    p->pc += 2;
    return addr;
}

word ABSX(cpu *p) {
    word addr = pReadAddr(p, p->pc);
    p->pc += 2;
    return addr + p->x;
}

word ABSY(cpu *p) {
    word addr = pReadAddr(p, p->pc);
    p->pc += 2;
    return addr + p->y;
}

word INDX(cpu *p) {
    byte zp_base = pReadCntr(p);
    byte zp = (zp_base + p->x) & 0xFF; 
    byte lo = pRead(p, p->mem + zp);
    byte hi = pRead(p, p->mem + ((zp + 1) & 0xFF));
    return (hi << 8) | lo;
}

word INDY(cpu *p) {
    byte zp = pReadCntr(p); 
    word lo = pRead(p, p->mem + zp);
    word hi = pRead(p, p->mem + ((zp + 1) & 0xFF)); 
    return ((hi << 8) | lo) + p->y;
}

void BRANCH(cpu *p, int cond) {
    int8_t off = (int8_t)pReadCntr(p);
    if (cond) {
        p->pc += off;
    }
}

void pStep(cpu *p) {

     
    byte op = pReadCntr(p);

    switch (op) {

    case 0xA9: LDA(p, IMM(p));  break;
    case 0xA5: LDA(p, ZP(p));   break;
    case 0xB5: LDA(p, ZPX(p));  break;
    case 0xAD: LDA(p, ABS(p));  break;
    case 0xBD: LDA(p, ABSX(p)); break;
    case 0xB9: LDA(p, ABSY(p)); break;
    case 0xA1: LDA(p, INDX(p)); break;
    case 0xB1: LDA(p, INDY(p)); break;

    case 0x85: STA(p, ZP(p));   break;
    case 0x95: STA(p, ZPX(p));  break;
    case 0x8D: STA(p, ABS(p));  break;
    case 0x9D: STA(p, ABSX(p)); break;
    case 0x99: STA(p, ABSY(p)); break;
    case 0x81: STA(p, INDX(p)); break;
    case 0x91: STA(p, INDY(p)); break;

    case 0xA2: LDX(p, IMM(p));  break;
    case 0xA6: LDX(p, ZP(p));   break;
    case 0xAE: LDX(p, ABS(p));  break;
    case 0x86: STX(p, ZP(p));   break;
    case 0x8C: STY(p, ABS(p));  break;
    case 0x8E: STX(p, ABS(p));  break;

    case 0xA0: LDY(p, IMM(p));  break;
    case 0xAC: LDY(p, ABS(p));  break;
    case 0x84: STY(p, ZP(p));   break;

    case 0x69: ADC(p, IMM(p));  break;
    case 0x65: ADC(p, ZP(p));   break;
    case 0x6D: ADC(p, ABS(p));  break;

    case 0xE9: SBC(p, IMM(p));  break;

    case 0x29: AND(p, IMM(p));  break;
    case 0x25: AND(p, ZP(p));   break;
    case 0x09: ORA(p, IMM(p));  break;
    case 0x49: EOR(p, IMM(p));  break;

    case 0xC9: CMP(p, IMM(p));  break;
    case 0xC5: CMP(p, ZP(p));   break;
    case 0xCD: CMP(p, ABS(p));  break;
    case 0xE0: CPX(p, IMM(p));  break;
    case 0xC0: CPY(p, IMM(p));  break;

    case 0x24: BIT(p, ZP(p));   break;
    case 0x2C: BIT(p, ABS(p));  break;

    case 0x0A: ASL_A(p);        break;
    case 0x06: ASL(p, ZP(p));   break;
    case 0x4A: LSR_A(p);        break;
    case 0x46: LSR(p, ZP(p));   break;
    case 0x2A: ROL(p, ABS(p));  break;
    case 0x6A: ROR(p, ABS(p));  break;

    case 0xE6: INC(p, ZP(p));   break;
    case 0xEE: INC(p, ABS(p));  break;
    case 0xC6: DEC(p, ZP(p));   break;
    case 0xCE: DEC(p, ABS(p));  break;
    
    case 0xE8: p->x++; update_zn(p, p->x); break;
    case 0xCA: p->x--; update_zn(p, p->x); break;
    case 0xC8: p->y++; update_zn(p, p->y); break;
    case 0x88: p->y--; update_zn(p, p->y); break;

    case 0xF0: BRANCH(p, (p->flags & 0x02));  break;
    case 0xD0: BRANCH(p, !(p->flags & 0x02)); break;
    case 0x10: BRANCH(p, !(p->flags & 0x80)); break;
    case 0x30: BRANCH(p, (p->flags & 0x80));  break;
    case 0x50: BRANCH(p, !(p->flags & 0x40)); break;
    case 0x70: BRANCH(p, (p->flags & 0x40));  break;
    case 0x90: BRANCH(p, !(p->flags & 0x01)); break;
    case 0xB0: BRANCH(p, (p->flags & 0x01));  break;

    case 0x4C: JMP(p, ABS(p));  break;
    case 0x6C: {
        word ptr = pReadAddr(p, p->pc);
        p->pc += 2;
        word lo = pRead(p, p->mem + ptr);
        word hi = pRead(p, p->mem + ((ptr & 0xFF00) | ((ptr + 1) & 0x00FF)));
        p->pc = (hi << 8) | lo;
        break;
    }

    case 0x20: JSR(p); break;
    case 0x60: RTS(p); break;

    case 0x48: PHA(p); break;
    case 0x68: PLA(p); break;
    case 0x08: PHP(p); break;
    case 0x28: PLP(p); break;

    case 0x00: BRK(p); break;
    case 0x40: RTI(p); break;
    case 0xEA: NOP();  break;

    case 0xAA: p->x = p->a; update_zn(p, p->x); break;
    case 0x8A: p->a = p->x; update_zn(p, p->a); break;
    case 0xA8: p->y = p->a; update_zn(p, p->y); break;
    case 0x98: p->a = p->y; update_zn(p, p->a); break;
    case 0x9A: p->sp = p->x; break;
    case 0xBA: p->x = p->sp; update_zn(p, p->x); break;

    default:
        break;
    }
}

void pRun(cpu *p) {
    for (;;) {
        pStep(p);
    }
}