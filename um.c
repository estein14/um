/*
    um.c 
    
    Written by: Emmett Stein (estein14), Noah Wright (nwrigh05)

    Purpose: Holds the main for the entire program. Calls the memory program
             with the file pointer, then calls the run program to execute 
             each instruction. Then exits. (Memory is deleted in halt because
             every program ends with halt)
 */
#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include "run.h"

int main(int argc, char*argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Need exactly two files in command line\n");
        exit(EXIT_FAILURE);
    }

    struct stat stats;
    stat(argv[1], &stats);
    
    int num_instructions = stats.st_size / 4;
    FILE *fp             = fopen(argv[1], "rb");

    if (fp == NULL) {
        fprintf(stderr, "Could not open file.\n");
        exit(EXIT_FAILURE);
    }

    Memory mem = Memory_new(fp, num_instructions);
    
    fclose(fp);
    
    int halt = runProgram(mem);
    if (halt) {
        Memory_free(mem);
    }
    exit(EXIT_SUCCESS);
}
