#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "memory.h"

void conditMove(uint32_t *reg, int rA, int rB, int rC);
void segLoad(Memory memory, uint32_t *reg, int rA, int rB, int rC);
void segStore(Memory memory, uint32_t *reg, int rA, int rB, int rC);
void addition(uint32_t *reg, int rA, int rB, int rC);
void multiplication(uint32_t *reg, int rA, int rB, int rC);
void division(uint32_t *reg, int rA, int rB, int rC);
void nand(uint32_t *reg, int rA, int rB, int rC);
void halt(Memory memory);
void mapSeg(Memory memory, uint32_t *reg, int rB, int rC);
void unmapSeg(Memory memory, uint32_t *reg, int rC);
void output(uint32_t *reg, int rC);
void input(uint32_t *reg, int rC);
void loadProgram();
void loadVal(uint32_t *reg, uint32_t val, uint32_t rA);




#endif
