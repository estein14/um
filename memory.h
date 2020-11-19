/*
    memory.h
    
    Written by: 




 */
#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/stat.h>
#include "bitpack.h"
#include "assert.h"
#include <seq.h>

typedef struct Memory {
    Seq_T segments;
    Seq_T reusable;
    int pSize;
}*Memory;


Memory Memory_new(FILE *fp, int num_instructions);
Seq_T readFile(FILE *fp, int num_instructions);
void Memory_free(Memory memory);
void mapSegment(Memory memory, int hint);


#endif
