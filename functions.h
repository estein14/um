#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "memory.h"

void conditMove(uint32_t *reg, int rA, int rB, int rC);
void segLoad(Memory memory, uint32_t *reg, int rA, int rB, int rC);
void segStore(Memory memory, uint32_t *reg, int rA, int rB, int rC);
void addition(uint32_t *reg, int rA, int rB, int rC);
void multiplication(uint32_t *reg, int rA, int rB, int rC);
void division(uint32_t *reg, int rA, int rB, int rC);





#endif