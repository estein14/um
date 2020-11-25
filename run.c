/*
    run.c

    Written by: Emmett Stein (estein14), Noah Wright (nwrigh05)

    Holds function definitions for running the actual while loop/program
 */
#include "run.h"

typedef uint32_t Um_instruction;
typedef enum Um_opcode {
    CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV, NAND, HALT, MAPSEG, UNMAPSEG,
    OUT, IN, LOADP, LV
} Um_opcode;

/*
    Arguments: bitpacked uint32_t instruction word
    Prupose: extracts the left most 4 bits ie. the OP code.
    Return: the extracted Um_opcode
    Invariants: none
 */
Um_opcode getOpcode(uint32_t instruction)
{
    Um_opcode instr = Bitpack_getu((uint64_t)instruction, 4, 28);
    return instr;
}

/*
    Arguments: Main memory structure
    Prupose: Iterates through the instructions of the 0th segment 
             and runs the corresponding instruction from each bitpacked
             word. 
    Return: void
    Invariants: none
 */
int runProgram(Memory memory)
{
    int pcounter           = 0;
    uint32_t reg[8]        = {0};
    Seg seg                = Seq_get(memory->seg, 0);
    uint32_t *instructions = seg->array;
    Um_opcode code;
    Um_instruction curr_instr;
    int rA, rB, rC, val, pcount;

    while (pcounter < memory->pSize) {
        curr_instr = instructions[pcounter];
        code       = getOpcode(curr_instr);
        rA         = Bitpack_getu((uint64_t)curr_instr, 3, 6);
        rB         = Bitpack_getu((uint64_t)curr_instr, 3, 3);
        rC         = Bitpack_getu((uint64_t)curr_instr, 3, 0);
        val        = Bitpack_getu((uint64_t)curr_instr, 25, 0);
        switch(code)
        {
            case CMOV:
                conditMove(reg, rA, rB, rC);
                break;
            case SLOAD:
                segLoad(memory, reg, rA, rB, rC);
                break;
            case SSTORE:
                segStore(memory, reg, rA, rB, rC);
                break;
            case ADD:
                addition(reg, rA, rB, rC);
                break;
            case MUL:
                multiplication(reg, rA, rB, rC);
                break;
            case DIV:
                division(reg, rA, rB, rC);
                break;
            case NAND:
                nand(reg, rA, rB, rC);
                break;
            case HALT:
                return 1;
            case MAPSEG:
                mapSeg(memory, reg, rB, rC);
                break;
            case UNMAPSEG:
                unmapSeg(memory, reg, rC);
                break;
            case OUT:
                output(reg, rC);
                break;
            case IN:
                input(reg, rC);
                break;
            case LOADP:
                pcount = loadProgram(memory, reg, rB, rC);
                pcounter = pcount - 1;
                seg = Seq_get(memory->seg, 0);
                instructions = seg->array;
                break;
            case LV:
                rA  = Bitpack_getu((uint64_t)curr_instr, 3, 25);
                loadVal(reg, val, rA);
                break;
        }
    pcounter++;
    }
    return 0;
}
