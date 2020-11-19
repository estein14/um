#include "functions.h"


void conditMove(uint32_t *reg, int rA, int rB, int rC)
{
    if (reg[rC] != 0) {
        reg[rA] = reg[rB];
    }
}

void segLoad(Memory memory, uint32_t *reg, int rA, int rB, int rC)
{
    
    uint32_t *segment = (uint32_t*)Seq_get(memory->segments, rB);
    uint32_t word = segment[rC];
    reg[rA] = word;

}

void segStore(Memory memory, uint32_t *reg, int rA, int rB, int rC)
{
    uint32_t *segment = (uint32_t*)Seq_get(memory->segments, rA);
    segment[rB] = reg[rC];
    
}

void addition(uint32_t *reg, int rA, int rB, int rC)
{
    //come abck and check for mod 2^32
    reg[rA] = (reg[rB] + reg[rC]);
}

void multiplication(uint32_t *reg, int rA, int rB, int rC)
{
    //come abck and check for mod 2^32
    reg[rA] = (reg[rB] * reg[rC]);
}

void division(uint32_t *reg, int rA, int rB, int rC)
{
    //come abck and check for mod 2^32
    //cant divide by 0
    reg[rA] = (reg[rB] / reg[rC]);
}
