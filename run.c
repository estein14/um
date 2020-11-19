/*
    run.c
    
    Written by: Emmett Stein (estein14), Noah Wright (nwrigh05)
    
    Holds function definitions for running the actual while loop/program


 */
 #include "run.h"
 
 typedef uint32_t Um_instruction;
 typedef enum Um_opcode {
         CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
         // NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
 } Um_opcode;
 
 
 Um_opcode getOpcode(uint32_t instruction)
 {
     Um_opcode instr = Bitpack_getu((uint64_t)instruction, 4, 28);
     return instr;
 }
 
 
 
 void runProgram(Memory memory)
 {
     /* size and coutner */
     uint32_t num_instructions = memory->pSize;
     uint32_t pcounter = 0;
     uint32_t reg[8];
     Um_opcode code;
     uint32_t curr_instr;
     int rA;
     int rB;
     int rC;
     uint32_t *instructions = (uint32_t*)Seq_get(memory->segments, 0);
     
     while (pcounter < num_instructions) {

        curr_instr = instructions[pcounter];
        code = getOpcode(curr_instr);

        rA = Bitpack_getu((uint64_t)curr_instr, 3, 6);
        rB = Bitpack_getu((uint64_t)curr_instr, 3, 3);
        rC = Bitpack_getu((uint64_t)curr_instr, 3, 0);
        
        printf("%d\n", code);
        
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
            // case NAND:
            // case HALT:
            // case ACTIVATE:
            // case INACTIVATE:
            // case OUT:
            // case IN:
            // case LOADP:
            // case LV:
            
        }
     //increment counter
     pcounter++;
    }
     
 }