#include "memory.h"

// constructor for our main memory
Memory Memory_new(FILE *fp, int num_instructions)
{
    assert(fp != NULL);

    Memory main = malloc(sizeof(*main));

    assert(main != NULL);

    main->segments = readFile(fp, num_instructions);
    main->reusable = Seq_new(0);
    main->pSize = num_instructions;
    main->ID = 0;

    return main;
}

void print_instructions(Memory memory, int num_instructions)
{

    uint32_t *array = (uint32_t *)Seq_get(memory->segments, 0);
    for (int i = 0; i < num_instructions; i++) {
        printf("%x\n", array[i]);
    }

}

void Memory_free(Memory memory)
{
    assert(memory != NULL);

    Seq_free(&(memory->reusable));

    int length = Seq_length(memory->segments);
    for (int i = 0; i < length; i++) {
        if (Seq_get(memory->segments, i) != NULL) {
            free(Seq_get(memory->segments, i));
        }
    }

    Seq_free(&(memory->segments));
    free(memory);

}

/*
    Reads file into first segment in sequence
 */
Seq_T readFile(FILE *fp, int num_instructions)
{


    // TODO: Check to make sure there are no remaining bits and all of the words
    // are the correct amount of bits
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


    Seq_addlo(obj, segment);


    return obj;
}

uint32_t mapSegment(Memory memory, int hint)
{

    //printf("Hint = %d\n", hint);
    //TODO: Check if reuasable elements are available

    uint32_t id;
    uint32_t *segment = malloc(sizeof(uint32_t) * hint);
    assert(segment != NULL);

    //might be able to use calloc here? if we use calloc instead of malloc i think all the values are automatically set to 0
    for (int i = 0; i < hint; i++) {
        segment[i] = 0;
    }

    if (Seq_length(memory->reusable) != 0) {
        //double check. Think it should be remLo cause we are adding lo in unmapsegment?
        id = (uint32_t)(uintptr_t)Seq_remhi(memory->reusable);
        Seq_put(memory->segments, id, segment);
    }
    else {
        Seq_addhi(memory->segments, segment);
        memory->ID++;
        id = memory->ID;
    }

    return id;

    // TODO: map bit pattern of non-zeros?

}


void unmapSegment(Memory memory, uint32_t id)
{
    // TODO: Error checking and bounds
    uint32_t *segment = (uint32_t*)Seq_get(memory->segments, id);
    free(segment);
    Seq_put(memory->segments, id, NULL);
    Seq_addhi(memory->reusable, (void*)(uintptr_t)id);
}

uint32_t segLength(Memory memory, uint32_t id)
{
    return *(uint32_t*)Seq_get(memory->segments, id);
}

