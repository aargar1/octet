#pragma once
#include "processor.h"

void update_zn(cpu *p, byte val);

void ORA(cpu *p, word addr);
void AND(cpu *p, word addr);
void EOR(cpu *p, word addr);
void ADC(cpu *p, word addr);
void STA(cpu *p, word addr);
void LDA(cpu *p, word addr);
void CMP(cpu *p, word addr);
void SBC(cpu *p, word addr);

void ASL(cpu *p, word addr);
void ASL_A(cpu *p);
void LSR(cpu *p, word addr);
void LSR_A(cpu *p);
void ROL(cpu *p, word addr);
void ROR(cpu *p, word addr);
void STX(cpu *p, word addr);
void LDX(cpu *p, word addr);
void DEC(cpu *p, word addr);
void INC(cpu *p, word addr);

void BIT(cpu *p, word addr);
void JMP(cpu *p, word addr);
void STY(cpu *p, word addr);
void LDY(cpu *p, word addr);
void CPY(cpu *p, word addr);
void CPX(cpu *p, word addr);

void NOP();
void TREG(cpu *p, byte *reg1, byte *reg2);
void SCLR(cpu *p, byte samt, byte val);

void PHA(cpu *p);
void PLA(cpu *p);
void PHP(cpu *p);
void PLP(cpu *p);

void BRK(cpu *p);
void JSR(cpu *p);
void RTI(cpu *p);
void RTS(cpu *p);