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

    FILE *fp = fopen(argv[1], "rb");

    if (fp == NULL) {
        fprintf(stderr, "Could not open file.\n");
        exit(EXIT_FAILURE);
    }

    Memory mem = Memory_new(fp, num_instructions);
    fclose(fp);


//    print_instructions(mem, num_instructions);



    runProgram(mem);

    exit(EXIT_SUCCESS);

}

/*
 * Checklist Tomorrow:
 * Unit testign for each function (thouroughly)
 *   FAULIRE STUFF - edge cases 
 * Put unit tests into file
 * COMB OVER SPEC
 * Function contracts
 * Clean up code
 * tabs and spaces and STYLE GUIDE
 * README
 * Final check and valgrind
 * SUBMIT!!
 */