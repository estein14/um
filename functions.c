#include "functions.h"


void conditMove(uint32_t *reg, int rA, int rB, int rC)
{
    if (reg[rC] != 0) {
        reg[rA] = reg[rB];
    }
}

void segLoad(Memory memory, uint32_t *reg, int rA, int rB, int rC)
{
    printf("In SegLoad\n");
    uint32_t *segment = (uint32_t*)Seq_get(memory->segments, rB);
    uint32_t word = segment[rC];
    reg[rA] = word;
    printf("Done\n");

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

void nand(uint32_t *reg, int rA, int rB, int rC)
{
    reg[rA] = ~(reg[rB] & reg[rC]);
}

void halt(Memory memory)
{
    Memory_free(memory);
    exit(EXIT_SUCCESS);
}

void mapSeg(Memory memory, uint32_t *reg, int rB, int rC)
{
    reg[rB] = mapSegment(memory, reg[rC]);
}

void unmapSeg(Memory memory, uint32_t *reg, int rC)
{
    unmapSegment( memory, reg[rC]);
}

void output(uint32_t *reg, int rC)
{
    putchar(reg[rC]);
}

void input(uint32_t *reg, int rC)
{
    int input = getchar();

    if(input == EOF) {
        input = ~0;
    }

    assert (input >= 0 || input <= 255);

    reg[rC] = input;
}

int loadProgram(Memory memory, uint32_t *reg, int rB, int rC)
{
    if (reg[rB] != 0) {
        /* Free old instructions */
        free(Seq_get(memory->segments, 0));
        uint32_t *new_seg = (uint32_t*)Seq_get(memory->segments, reg[rB]);
        Seq_put(memory->segments, 0, new_seg);
        
        //uint32_t length = *(uint32_t*)Seq_get(new_seg, reg[rB])
        //TODO: Finish function
        
    } 
    return reg[rC];
    
}

void loadVal(uint32_t *reg, uint32_t val, uint32_t rA)
{
    printf("In load\n");
    reg[rA] = val;
}
