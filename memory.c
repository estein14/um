#include "memory.h"

// constructor for our main memory
Memory Memory_new(FILE *fp, int num_instructions)
{
    assert(fp != NULL);

    Memory main = malloc(sizeof(*main));

    assert(main != NULL);

    main->seg = readFile(fp, num_instructions);
    main->reusable = Seq_new(0);
    main->pSize = num_instructions;
    main->ID = 0;

    return main;
}

void print_instructions(Memory memory, int num_instructions)
{

    Seg seg = Seq_get(memory->seg, 0);
    uint32_t *array = seg->array;
    for (int i = 0; i < num_instructions; i++) {
        printf("%x\n", array[i]);
    }

}

void Memory_free(Memory memory)
{
    assert(memory != NULL);

    Seq_free(&(memory->reusable));

    int length = Seq_length(memory->seg);
    Seg seg;
    
    for (int i = 0; i < length; i++) {
        if (Seq_get(memory->seg, i) != NULL) {
            seg = Seq_get(memory->seg, i);
            free(seg->array);
            free(seg);
        }
    }

    Seq_free(&(memory->seg));
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
    Seg seg = malloc(sizeof(*seg));
    assert(seg != NULL);

    uint32_t *array = malloc(sizeof(uint32_t) * num_instructions);
    uint32_t word;

    for (int i = 0; i < num_instructions; i++) {
        for (int j = 24; j >= 0; j-=8) {
            int c = getc(fp);
            word  = Bitpack_newu(word, 8, j, c);
        }
        array[i] = (uint32_t)word;
    }
    seg->array  = array;
    seg->length = num_instructions;
    Seq_addlo(obj, seg);

    return obj;
}

uint32_t mapSegment(Memory memory, int hint)
{

    //TODO: Check if reuasable elements are available

    uint32_t id;
    uint32_t *array = calloc(hint, sizeof(uint32_t));
    Seg seg = malloc(sizeof(*seg));
    assert(seg != NULL);
    assert(array != NULL);
    
    //might be able to use calloc here? if we use calloc instead of malloc i think all the values are automatically set to 0
    for (int i = 0; i < hint; i++) {
        array[i] = 0;
    }
    
    seg->length = hint;
    seg->array  = array;

    if (Seq_length(memory->reusable) != 0) {
        //double check. Think it should be remLo cause we are adding lo in unmapsegment?
        id = (uint32_t)(uintptr_t)Seq_remhi(memory->reusable);
        Seq_put(memory->seg, id, seg);
    }
    else {
        Seq_addhi(memory->seg, seg);
        memory->ID++;
        id = memory->ID;
    }

    return id;

    // TODO: map bit pattern of non-zeros?

}


void unmapSegment(Memory memory, uint32_t id)
{
    // TODO: Error checking and bounds
    Seg seg = Seq_get(memory->seg, id);
    uint32_t *array = seg->array;
    free(array);
    free(seg);
    Seq_put(memory->seg, id, NULL);
    Seq_addhi(memory->reusable, (void*)(uintptr_t)id);
}

uint32_t segLength(Memory memory, uint32_t id)
{
    Seg seg = Seq_get(memory->seg, id);
    uint32_t length = seg->length;
    return length;
}

