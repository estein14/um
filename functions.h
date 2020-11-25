/*
    functions.h
    
    Written by: Noah Wright (nwrigh05), Emmett Stein (estein14)
    
    Purpose: File holds all of the function declerations for functions.c
             All the 14 different operations that the UM can compute
 */
#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "memory.h"

/* All of the declarations for the different 14 possible operations */
void conditMove(uint32_t *reg, int rA, int rB, int rC);
void segLoad(Memory memory, uint32_t *reg, int rA, int rB, int rC);
void segStore(Memory memory, uint32_t *reg, int rA, int rB, int rC);
void addition(uint32_t *reg, int rA, int rB, int rC);
void multiplication(uint32_t *reg, int rA, int rB, int rC);
void division(uint32_t *reg, int rA, int rB, int rC);
void nand(uint32_t *reg, int rA, int rB, int rC);
void mapSeg(Memory memory, uint32_t *reg, int rB, int rC);
void unmapSeg(Memory memory, uint32_t *reg, int rC);
void output(uint32_t *reg, int rC);
void input(uint32_t *reg, int rC);
int  loadProgram(Memory memory, uint32_t *reg, int rB, int rC);
void loadVal(uint32_t *reg, uint32_t val, uint32_t rA);

#endif
