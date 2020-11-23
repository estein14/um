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
    


//    print_instructions(mem, num_instructions);



    runProgram(mem);

    exit(EXIT_SUCCESS);

}

/*Checklist Tomorrow:
* Finish function implementations
        CHECK FUNCTIONALITY OF EACH INDIVIDUALLY
* Boundary checking / program failures
* 
* 


* Function Contracts
* Readme


 */