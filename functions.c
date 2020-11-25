/*
    functions.c
    
    Written by: Noah Wright (nwrigh05), Emmett Stein (estein14)
    
    Purpose: File holds all of the functions that are called from run program.
             Each functions executes one of the 14 possible opeations that 
             the UM can execute.
 */
#include "functions.h"

/*
    Arguments: registers array, ints rABC to know which register to use
    Purpose: executes the conditional move command
    Return: void
    Invariants: none
 */
void conditMove(uint32_t *reg, int rA, int rB, int rC)
{
    if (reg[rC] != 0) {
        reg[rA] = reg[rB];
    }
}

/*
    Arguments: Main memory, registers array, 
               ints rABC to know which register to use
    Purpose: executes segemnted load function. Takes a word from memory and 
             stores it in our first 0'th register
    Return: void
    Invariants: none
 */
void segLoad(Memory memory, uint32_t *reg, int rA, int rB, int rC)
{
    Seg seg         = Seq_get(memory->seg, reg[rB]);
    uint32_t *array = seg->array;
    uint32_t word   = array[reg[rC]];
    reg[rA]         = word;
}

/*
    Arguments: Main memory, registers array, 
               ints rABC to know which register to use
    Purpose: executes segemnted load function. Takes a word from memory and 
             stores it in our first 0'th register
    Return: void
    Invariants: none
 */
void segStore(Memory memory, uint32_t *reg, int rA, int rB, int rC)
{
    Seg seg         = Seq_get(memory->seg, reg[rA]);
    uint32_t *array = seg->array;
    array[reg[rB]]  = reg[rC];

}

/*
    Arguments: registers array, ints rABC to know which register to use
    Purpose: add's two numbers from the registers and stores the sum in rA
    Return: void
    Invariants: none
 */
void addition(uint32_t *reg, int rA, int rB, int rC)
{
    reg[rA] = (reg[rB] + reg[rC]);
}

/*
    Arguments: registers array, ints rABC to know which register to use
    Purpose: multiplies's two numbers from the registers and stores the 
             sum in rA
    Return: void
    Invariants: none
 */
void multiplication(uint32_t *reg, int rA, int rB, int rC)
{
    reg[rA] = (reg[rB] * reg[rC]);
}

/*
    Arguments: registers array, ints rABC to know which register to use
    Purpose: divides's two numbers from the registers and stores the sum in rA
    Return: void
    Invariants: none
 */
void division(uint32_t *reg, int rA, int rB, int rC)
{
    reg[rA] = (reg[rB] / reg[rC]);
}

/*
    Arguments: registers array, ints rABC to know which register to use
    Purpose: executes the nand move command
             r[A] = ~(r[B] & r[C])
    Return: void
    Invariants: none
 */
void nand(uint32_t *reg, int rA, int rB, int rC)
{
    reg[rA] = ~(reg[rB] & reg[rC]);
}

/*
    Wrapper function to call mapSegemnt in memory.c
 */
void mapSeg(Memory memory, uint32_t *reg, int rB, int rC)
{
    reg[rB] = mapSegment(memory, reg[rC]);
}

/*
    Wrapper function to call unmapSegament in memory.c
 */
void unmapSeg(Memory memory, uint32_t *reg, int rC)
{
    unmapSegment(memory, reg[rC]);
}

/*
    Arguments: registers, int rC
    Purpose: Given an index and the registers, it prints to stdout the value
             (as a char) in reg[rC]
    Return: void
    Invariants: must be between 0 and 255 (uint32_t so always greater than 0)
 */
void output(uint32_t *reg, int rC)
{
    assert (reg[rC] <= 255);
    putchar(reg[rC]);
}

/*
    Arguments: registers,int rC
    Purpose: calls standard input to put in value, and places that value into 
             our register at index rC.
    Return: void
    Invariants: input must be between 0 and 255
 */
void input(uint32_t *reg, int rC)
{
    int input = getchar();
    if(input == EOF) {
        input = ~0;
    }
    assert (input >= 0 && input <= 255);
    reg[rC] = input;
}

/*
    Arguments: Main memory, registers, ints rB rC
    Purpose: Finds a certain segment in memory, and creates a deep copy. It 
             deletes the current instructions, and replaces them with the new 
             duplicated segment. Then sets the program counter and size to the 
             new values from memory and continues running
    Return: void (however, after should have full new set of instructions)
    Invariants: None
 */
int loadProgram(Memory memory, uint32_t *reg, int rB, int rC)
{
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

/*
    Arguments: registers array, value to upload, register to upload it to
    Purpose: loads the value from val into the correct register
    Return: void
    Invariants: none
 */
void loadVal(uint32_t *reg, uint32_t val, uint32_t rA)
{
    reg[rA] = val;
}
