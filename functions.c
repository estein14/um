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
    Seg seg = Seq_get(memory->seg, reg[rB]);
    uint32_t *array = seg->array;
    uint32_t word = array[reg[rC]];
    reg[rA] = word;

}

void segStore(Memory memory, uint32_t *reg, int rA, int rB, int rC)
{
    // printf("In Segstore\n");
    Seg seg = Seq_get(memory->seg, reg[rA]);
    uint32_t *array = seg->array;
    array[reg[rB]] = reg[rC];

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
    unmapSegment(memory, reg[rC]);
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
    //fprintf(stderr, "%d\n", reg[rB]);
    if (reg[rB] != 0) {
        
        uint32_t length   = segLength(memory, reg[rB]);
        Seg old           = Seq_get(memory->seg, reg[rB]);
        uint32_t *old_arr = old->array;
        
        Seg seg           = malloc(sizeof(*seg));
        uint32_t *new_arr = malloc(sizeof(uint32_t) * length);
        assert(seg != NULL);
        assert(new_arr != NULL);
        
        /* Free old instructions */
        Seg old_ins             = Seq_get(memory->seg, 0);
        uint32_t *old_arr_instr = old_ins->array;
        free(old_arr_instr);
        free(old_ins);
                
        for (uint32_t i = 0; i < length; i++) {
            new_arr[i] = old_arr[i];
        }
        
        seg->array = new_arr;
        Seq_put(memory->seg, 0, seg);
        
        memory->pSize = length;
        
    } 
    return reg[rC];
}

void loadVal(uint32_t *reg, uint32_t val, uint32_t rA)
{
    reg[rA] = val;
}
