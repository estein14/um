#include <stdio.h>
#include <stdlib.h>
#include "memory.h"

int main(int argc, char*argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Need exactly two files in command line\n");
        exit(EXIT_FAILURE);
    }

    struct stat stats;
    stat(argv[1], &stats);
    int num_instructions = stats.st_size / 4;

    File *fp = fopen(argv[1], "rb");

    if (fp == NULL) {
        fprintf(stderr, "Could not open file.\n")
        exit(EXIT_FAILURE);
    }

    Memory mem = Memory_new(fp, num_instructions);

    Run_program(mem);

    Memory_clear(&mem);
    exit(EXIT_SUCCESS);

}
