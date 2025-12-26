#include "includes/opcode.h"

void update_zn(cpu *p, byte val) {
    p->flags &= 0x7D;
    if (val == 0) p->flags |= 0x02;
    if (val & 0x80) p->flags |= 0x80;
}

void ORA(cpu *p, word addr) {
    p->a |= pRead(p, p->mem + addr);
    update_zn(p, p->a);
}

void AND(cpu *p, word addr) {
    p->a &= pRead(p, p->mem + addr);
    update_zn(p, p->a);
}

void EOR(cpu *p, word addr) {
    p->a ^= pRead(p, p->mem + addr);
    update_zn(p, p->a);
}

void ADC(cpu *p, word addr) {
    byte m = pRead(p, p->mem + addr);
    word res = p->a + m + (p->flags & 0x01);
    byte v = (~(p->a ^ m) & (p->a ^ (res & 0xFF)) & 0x80) != 0;
    
    p->a = res & 0xFF;
    
    p->flags &= 0x3C;
    if (p->a & 0x80) p->flags |= 0x80;
    if (v) p->flags |= 0x40;
    if (p->a == 0) p->flags |= 0x02;
    if (res > 0xFF) p->flags |= 0x01;
}

void STA(cpu *p, word addr) {
    pWrite(p, p->mem + addr, p->a);
}

void LDA(cpu *p, word addr) {
    p->a = pRead(p, p->mem + addr);
    update_zn(p, p->a);
}

void CMP(cpu *p, word addr) {
    byte m = pRead(p, p->mem + addr);
    int res = (int)p->a - (int)m; 
    
    p->flags &= 0x7C;
    
    if (p->a >= m) p->flags |= 0x01;       
    if ((res & 0xFF) == 0) p->flags |= 0x02; 
    if (res & 0x80) p->flags |= 0x80;
}

void SBC(cpu *p, word addr) {
    byte m = pRead(p, p->mem + addr);
    word res = (word)p->a + (word)(m ^ 0xFF) + (word)(p->flags & 0x01);
    
    if (((p->a ^ res) & (p->a ^ (m ^ 0xFF)) & 0x80)) 
        p->flags |= 0x40;
    else 
        p->flags &= ~0x40;

    p->a = res & 0xFF;
    update_zn(p, p->a);

    if (res > 0xFF) p->flags |= 0x01;
    else p->flags &= ~0x01;
}

void ASL(cpu *p, word addr) {
    byte val = pRead(p, p->mem + addr);
    p->flags = (p->flags & 0xFE) | ((val & 0x80) >> 7);
    val <<= 1;
    pWrite(p, p->mem + addr, val);
    update_zn(p, val);
}

void ASL_A(cpu *p) {
    p->flags = (p->flags & 0xFE) | ((p->a & 0x80) >> 7);
    p->a <<= 1;
    update_zn(p, p->a);
}

void LSR(cpu *p, word addr) {
    byte val = pRead(p, p->mem + addr);
    p->flags = (p->flags & 0xFE) | (val & 0x01);
    val >>= 1;
    pWrite(p, p->mem + addr, val);
    update_zn(p, val);
}

void LSR_A(cpu *p) {
    p->flags = (p->flags & 0xFE) | (p->a & 0x01);
    p->a >>= 1;
    update_zn(p, p->a);
}

void ROL(cpu *p, word addr) {
    byte val = pRead(p, p->mem + addr);
    byte old_carry = (p->flags & 0x01);
    
    p->flags &= 0xFE; 
    if (val & 0x80) p->flags |= 0x01;
    
    val = (val << 1) | old_carry;
    pWrite(p, p->mem + addr, val);
    update_zn(p, val);
}

void ROR(cpu *p, word addr) {
    byte val = pRead(p, p->mem + addr);
    byte carry = (p->flags & 0x01) << 7;
    p->flags = (p->flags & 0xFE) | (val & 0x01);
    val = (val >> 1) | carry;
    pWrite(p, p->mem + addr, val);
    update_zn(p, val);
}

void STX(cpu *p, word addr) {
    pWrite(p, p->mem + addr, p->x);
}

void LDX(cpu *p, word addr) {
    p->x = pRead(p, p->mem + addr);
    update_zn(p, p->x);
}

void DEC(cpu *p, word addr) {
    byte val = pRead(p, p->mem + addr) - 1;
    pWrite(p, p->mem + addr, val);
    update_zn(p, val);
}

void INC(cpu *p, word addr) {
    byte val = pRead(p, p->mem + addr) + 1;
    pWrite(p, p->mem + addr, val);
    update_zn(p, val);
}

void BIT(cpu *p, word addr) {
    byte m = pRead(p, p->mem + addr);
    p->flags &= 0x3D;
    p->flags |= (m & 0xC0);
    if ((m & p->a) == 0) p->flags |= 0x02;
}

void JMP(cpu *p, word addr) {
    p->pc = addr;
}

void STY(cpu *p, word addr) {
    pWrite(p, p->mem + addr, p->y);
}

void LDY(cpu *p, word addr) {
    p->y = pRead(p, p->mem + addr);
    update_zn(p, p->y);
}

void CPY(cpu *p, word addr) {
    byte m = pRead(p, p->mem + addr);
    word res = p->y - m;
    p->flags &= 0x7C;
    if (p->y >= m) p->flags |= 0x01;
    if ((res & 0xFF) == 0) p->flags |= 0x02;
    if (res & 0x80) p->flags |= 0x80;
}

void CPX(cpu *p, word addr) {
    byte m = pRead(p, p->mem + addr);
    int res = (int)p->x - (int)m;
    p->flags &= 0x7C;
    if (res >= 0) p->flags |= 0x01;
    if ((res & 0xFF) == 0) p->flags |= 0x02;
    if (res & 0x80) p->flags |= 0x80;
}

void NOP() {
    return;
}

void PHA(cpu *p) {
    pWrite(p, p->mem + 0x100 + p->sp, p->a);
    p->sp--;
}

void PLA(cpu *p) {
    p->sp++;
    p->a = pRead(p, p->mem + 0x100 + p->sp);
    update_zn(p, p->a);
}

void PHP(cpu *p) {
    pWrite(p, p->mem + 0x100 + p->sp, p->flags | 0x30);
    p->sp--;
}

void PLP(cpu *p) {
    p->sp++;
    p->flags = (pRead(p, p->mem + 0x100 + p->sp) & 0xEF) | 0x20;
}

void JSR(cpu *p) {
    word lo = p->mem[p->pc];
    word hi = p->mem[p->pc + 1];
    word target = (hi << 8) | lo;

    word returnAddr = p->pc + 1; 

    pWrite(p, p->mem + 0x100 + p->sp, (returnAddr >> 8) & 0xFF);
    p->sp--;
    pWrite(p, p->mem + 0x100 + p->sp, returnAddr & 0xFF);
    p->sp--;

    p->pc = target;
}

void RTS(cpu *p) {
    p->sp++;
    word lo = pRead(p, p->mem + 0x100 + p->sp);
    p->sp++;
    word hi = pRead(p, p->mem + 0x100 + p->sp);
    p->pc = ((hi << 8) | lo) + 1;
}

void BRK(cpu *p) {
    p->pc++;
    pWrite(p, p->mem + 0x100 + p->sp, (p->pc >> 8) & 0xFF);
    p->sp--;
    pWrite(p, p->mem + 0x100 + p->sp, p->pc & 0xFF);
    p->sp--;
    pWrite(p, p->mem + 0x100 + p->sp, p->flags | 0x30);
    p->sp--;
    p->flags |= 0x04;
    p->pc = pReadAddr(p, 0xFFFE);
}

void RTI(cpu *p) {
    p->sp++;
    p->flags = (pRead(p, p->mem + 0x100 + p->sp) & 0xEF) | 0x20;
    p->sp++;
    word lo = pRead(p, p->mem + 0x100 + p->sp);
    p->sp++;
    word hi = pRead(p, p->mem + 0x100 + p->sp);
    p->pc = (hi << 8) | lo;
}