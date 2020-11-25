/*
 * umlab.c
 *
 * Functions to generate UM unit tests. Once complete, this module
 * should be augmented and then linked against umlabwrite.c to produce
 * a unit test writing program.
 *
 * A unit test is a stream of UM instructions, represented as a Hanson
 * Seq_T of 32-bit words adhering to the UM's instruction format.
 *
 * Any additional functions and unit tests written for the lab go
 * here.
 *
 */


#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <seq.h>
#include <bitpack.h>


typedef uint32_t Um_instruction;
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;


/* Functions that return the two instruction types */

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc);
Um_instruction loadval(unsigned ra, unsigned val);


/* Wrapper functions for each of the instructions */

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;

static inline Um_instruction conditMove(Um_register a, Um_register b, 
                                        Um_register c)
{
    return three_register(0, a, b, c);
}

static inline Um_instruction segLoad(Um_register a, Um_register b, 
                                     Um_register c)
{
    return three_register(SLOAD, a, b, c);
}

static inline Um_instruction segStore(Um_register a, Um_register b, 
                                      Um_register c)
{
    return three_register(SSTORE, a, b, c);
}

static inline Um_instruction add(Um_register a, Um_register b, Um_register c)
{
        return three_register(ADD, a, b, c);
}

static inline Um_instruction multiply(Um_register a, Um_register b, 
                                      Um_register c)
{
        return three_register(MUL, a, b, c);
}

static inline Um_instruction division(Um_register a, Um_register b, 
                                      Um_register c)
{
        return three_register(DIV, a, b, c);
}

static inline Um_instruction nand(Um_register a, Um_register b, Um_register c)
{
        return three_register(NAND, a, b, c);
}

static inline Um_instruction halt(void)
{
        return three_register(HALT, 0, 0, 0);
}

static inline Um_instruction mapseg(Um_register b, Um_register c)
{
        return three_register(ACTIVATE, 0, b, c);
}

static inline Um_instruction unmapSeg(Um_register c)
{
        return three_register(INACTIVATE, 0, 0, c);
}

static inline Um_instruction output(Um_register c)
{
    return three_register(OUT, 0, 0, c);
}

static inline Um_instruction input(Um_register c)
{
    return three_register(IN, 0, 0, c);
}

static inline Um_instruction loadProgram(Um_register b, Um_register c)
{
    return three_register(LOADP, 0, b, c);
}


/* Functions for working with streams */

static inline void append(Seq_T stream, Um_instruction inst)
{
        assert(sizeof(inst) <= sizeof(uintptr_t));
        Seq_addhi(stream, (void *)(uintptr_t)inst);
}

const uint32_t Um_word_width = 32;

Um_instruction three_register(Um_opcode op, int ra, int rb, int rc)
{
    uint64_t instructions = 0;

    instructions = Bitpack_newu(instructions, 4, 28, op);
    instructions = Bitpack_newu(instructions, 3, 6, ra);
    instructions = Bitpack_newu(instructions, 3, 3, rb);
    instructions = Bitpack_newu(instructions, 3, 0, rc);

    return (uint32_t)instructions;
}

Um_instruction loadval(unsigned ra, unsigned val)
{
    uint64_t instructions = 0;

    instructions = Bitpack_newu(instructions, 4, 28, 13);
    instructions = Bitpack_newu(instructions, 3, 25, ra);
    instructions = Bitpack_newu(instructions, 25, 0, val);

    return (uint32_t)instructions;
}

void Um_write_sequence(FILE *output, Seq_T stream)
{
        assert(output != NULL && stream != NULL);
        int stream_length = Seq_length(stream);
        for (int i = 0; i < stream_length; i++) {
                Um_instruction inst = (uintptr_t)Seq_remlo(stream);
                for (int lsb = Um_word_width - 8; lsb >= 0; lsb -= 8) {
                        fputc(Bitpack_getu(inst, 8, lsb), output);
                }
        }

}


/* Unit tests for the UM */

void build_halt_test(Seq_T stream)
{
        append(stream, halt());
}

void build_verbose_halt_test(Seq_T stream)
{
    append(stream, halt());
    append(stream, loadval(r1, 'B'));
    append(stream, output(r1));
    append(stream, loadval(r1, 'a'));
    append(stream, output(r1));
    append(stream, loadval(r1, 'd'));
    append(stream, output(r1));
    append(stream, loadval(r1, '!'));
    append(stream, output(r1));
    append(stream, loadval(r1, '\n'));
    append(stream, output(r1));
}

void build_add_halt_test(Seq_T stream)
{
    append(stream, loadval(r2, 45));
    append(stream, loadval(r3, 60));
    append(stream, add(r1, r2, r3));
    append(stream, output(r1));
    append(stream, halt());
}

void build_print_digit_test(Seq_T stream)
{
    append(stream, loadval(r1, 48));
    append(stream, loadval(r2, 6));

    append(stream, add(r3, r1, r2));

    append(stream, output(r3));
    append(stream, halt());
}

void build_condit_move(Seq_T stream)
{
    append(stream, loadval(0, 10));
    append(stream, loadval(1, 35));
    append(stream, loadval(2, 3));
    append(stream, conditMove(0, 1, 2));
    append(stream, output(0));
    append(stream, halt());
}

void build_map_seg(Seq_T stream)
{
    append(stream, loadval(2, 10));
    append(stream, mapseg(0, 2));
    append(stream, loadval(2, 35));
    append(stream, loadval(0, 1));
    append(stream, loadval(1, 0));
    append(stream, segStore(0, 1, 2));
    append(stream, loadval(0, 0));
    append(stream, loadval(1, 1));
    append(stream, loadval(2, 0));
    append(stream, segLoad(0, 1, 2));
    append(stream, output(0));
    append(stream, halt());
}

void build_map_seg_2(Seq_T stream)
{
    //make a new seg with 10 spots
    append(stream, loadval(2, 10));
    append(stream, mapseg(0, 2));
    
    //putting m[1][0] = 35
    append(stream, loadval(2, 35));
    append(stream, loadval(0, 1));
    append(stream, loadval(1, 0));
    append(stream, segStore(0, 1, 2));
    
    //map new segment with 7 spots
    append(stream, loadval(2, 7));
    append(stream, mapseg(0, 2));
    
    // puts m[2][3] = 36
    append(stream, loadval(2, 36));
    append(stream, loadval(0, 2));
    append(stream, loadval(1, 3));
    append(stream, segStore(0, 1, 2));
    
    //putting r[5] = m[2][3]
    append(stream, loadval(r5, 0));
    append(stream, loadval(r6, 2));
    append(stream, loadval(r7, 3));
    append(stream, segLoad(r5, r6, r7));
    append(stream, output(r5));
    
    //putting r[0] = m[1][0]
    append(stream, loadval(0, 0));
    append(stream, loadval(1, 1));
    append(stream, loadval(2, 0));
    append(stream, segLoad(0, 1, 2));
    append(stream, output(0));
    
    //unmap segment 1
    append(stream, loadval(r4, 1));
    append(stream, unmapSeg(r4));
    
    append(stream, loadval(r3, 6));
    append(stream, mapseg(0, r3));
    
    // puts m[1][5] = 90
    append(stream, loadval(r4, 1));
    append(stream, loadval(r5, 5));
    append(stream, loadval(r6, 90));
    append(stream, segStore(r4, r5, r6));
    
    //putting r[2] = m[1][5]
    append(stream, loadval(r5, 0));
    append(stream, loadval(r6, 1));
    append(stream, loadval(r7, 5));
    append(stream, segLoad(r5, r6, r7));
    append(stream, output(r5));
    
    append(stream, loadval(r5, 2));
    append(stream, unmapSeg(r5));
    
    append(stream, halt());
}

void build_multiply_test(Seq_T stream)
{
    append(stream, loadval(r3, 17));
    append(stream, loadval(r6, 5));
    append(stream, multiply(r2, r3, r6));
    append(stream, output(r2)); //Should print upper-case 'U'
    append(stream, halt());
}

void build_divide_test(Seq_T stream)
{
    append(stream, loadval(r3, 240));
    append(stream, loadval(r6, 2));
    append(stream, division(r2, r3, r6));
    append(stream, output(r2)); //Should print upper-case 'x'
    append(stream, halt());
}

void build_divide_0(Seq_T stream)
{
    append(stream, loadval(r3, 100));
    append(stream, loadval(r4, 0));
    append(stream, division(r1, r3, r4)); // cannot divide by 0
    append(stream, halt());
}

void build_nand_test(Seq_T stream)
{
    append(stream, loadval(r0, 65535));
    append(stream, loadval(r1, 65537));
    append(stream, multiply(r2, r0, r1)); 
        
    append(stream, loadval(r3, 65542));
    append(stream, loadval(r4, 65530));
    append(stream, multiply(r5, r4, r3)); 
        
    append(stream, nand(r6, r2, r5));
    append(stream, output(r6)); /* print out # */
    append(stream, halt());
}

void build_load_program(Seq_T stream)
{
    append(stream, loadval(r0, 100)); 
    append(stream, nand(r0, r0, r0)); 
    append(stream, loadval(r2, 35));

    append(stream, loadval(r1, 0));
    append(stream, loadval(r3, 1));
    append(stream, loadval(r4, 6)); 

    append(stream, loadval(r5, 11)); 
    append(stream, output(r2));
    append(stream, add(r0, r0, r3)); 
    append(stream, conditMove(r5, r4, r0)); 
    append(stream, loadProgram(r1, r5)); 

    append(stream, halt());
}

void build_50_mill(Seq_T stream)
{
    append(stream, loadval(r0, 12499998)); 
    append(stream, nand(r0, r0, r0)); 

    append(stream, loadval(r1, 0));
    append(stream, loadval(r3, 1));
    append(stream, loadval(r4, 5)); 

    append(stream, loadval(r5, 9)); 
    append(stream, add(r0, r0, r3)); 
    append(stream, conditMove(r5, r4, r0)); 
    append(stream, loadProgram(r1, r5)); 

    append(stream, halt());
}

void build_test_input(Seq_T stream)
{
    append(stream, input(r2));
    append(stream, output(r2));
    append(stream, halt());
}

