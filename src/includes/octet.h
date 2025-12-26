#pragma once
#include "opcode.h"
#include "processor.h"
#include <stdio.h>

void pStep(cpu *p);
void pRun(cpu *p);