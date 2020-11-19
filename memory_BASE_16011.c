#include "memory.h"

typedef struct Memory {
    Seq_T segments;
    Seq_T reusable;
    int pSize;
} Memory;

// constructor for our main memory
Memory Memory_new(File *fp, int num_instructions)
{
    assert(fp != NULL);

    Memory main = malloc(sizeof(*main));

    assert(main != NULL);

    main->segments = readFile(fp);
    main->reusable = Seq_new(0);
    main->pSize = num_instructions;

    return main;
}


/*
    Reads file into first segment in sequence
 */
Seq_T readFile(File *fp, int num_instructions)
{
    Seq_T obj = Seq_new(0);
    uint32_t *segment = malloc(sizeof(uint32_t) * num_instructions);
    uint32_t word;

    for (int i = 0; i < num_instructions; i++) {
        for (int j = 24; j >= 0; j-=8) {
            int c = getc(fp);
            word = Bitpack_newu(word, 8, j, c);
        }
        segment[i] = (uint32_t)word;
    }

    Seq_put(obj, 0, segment);

    return obj;
}
