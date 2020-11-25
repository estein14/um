/*
    memory.h

    Written by: Emmett Stein (estein14), noah Wright (nwrigh05)
    
    purpose: House all the function declerations for the functions that
             interact with our memory structure.
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

/*
Memory architecture: 
        Main memory struct:
            Sequence of seg structs
                seg structs include:
                    the uint32_t array of words
                    length of that array
        instruction size
        ID counter to keep track of
 */
 
/* main memory struct to hold aspects of memory */
typedef struct Memory {
    Seq_T seg;
    Seq_T reusable;
    int pSize;
    uint32_t ID;
}*Memory;

/* segment struct that contains each array of words and their lengths */
typedef struct Seg {
    uint32_t length;
    uint32_t *array;
}*Seg;

/* Reads in file and returns new structure with all the instruction */
Memory Memory_new(FILE *fp, int num_instructions);

/* called form memory_new, helps read the file and parse up the data */
Seq_T readFile(FILE *fp, int num_instructions);

/* Frees the memory in our main memory structure */
void Memory_free(Memory memory);

/* Maps new segment of size length and puts it in either a reused place or 
   adds it high to the sequence  */
uint32_t mapSegment(Memory memory, int hint);

/* Given an index, deletes memory at that index (sets to NULL), 
   adds it to reusable indices sequence */
void unmapSegment(Memory memory, uint32_t id);

/* Given an index in main memory, returns the length of the array */
uint32_t segLength(Memory memory, uint32_t id);

#endif
