#include "functions.h"


void conditMove(uint32_t *reg, int rA, int rB, int rC)
{
    if (reg[rC] != 0) {
        reg[rA] = reg[rB];
    }
}

void segLoad(Memory memory, uint32_t *reg, int rA, int rB, int rC)
{
    // printf("In SegLoad\n");
    uint32_t *segment = (uint32_t*)Seq_get(memory->segments, reg[rB]);
    uint32_t word = segment[reg[rC]];
    reg[rA] = word;

}

void segStore(Memory memory, uint32_t *reg, int rA, int rB, int rC)
{
    // printf("In Segstore\n");
    uint32_t *segment = (uint32_t*)Seq_get(memory->segments, reg[rA]);
    segment[reg[rB]] = reg[rC];

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
    // printf("In Mapseg\n");
    reg[rB] = mapSegment(memory, reg[rC]);
}

void unmapSeg(Memory memory, uint32_t *reg, int rC)
{
    // printf("In Unmap\n");
    unmapSegment( memory, reg[rC]);
}

void output(uint32_t *reg, int rC)
{
    assert(reg[rC] <= 255);
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
    fprintf(stderr, "%d\n", reg[rB]);
    // length + 1 somewhere??
    if (reg[rB] != 0) {
        /* Free old instructions */
        free(Seq_get(memory->segments, 0));
        /* length of new segment */
        uint32_t length = segLength(memory, reg[rB]);
        fprintf(stderr, "Length = %d\n", length);
        uint32_t *new = calloc(length, sizeof(uint32_t));
        assert(new != NULL);
        
        uint32_t *to_copy = (uint32_t*)Seq_get(memory->segments, reg[rB]);
        
        for (uint32_t i = 0; i < length; i++) {
            new[i] = to_copy[i];
        }
        
        Seq_put(memory->segments, 0, new);
        
    } 
    return reg[rC];
    
}

void loadVal(uint32_t *reg, uint32_t val, uint32_t rA)
{
    reg[rA] = val;
}
