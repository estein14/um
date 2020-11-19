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
    Seq_T obj = Seq_new(10);

    uint32_t *segment = malloc(sizeof(uint32_t) * num_instructions);
    uint32_t word;

    
    for (int i = 0; i < num_instructions; i++) {
        for (int j = 24; j >= 0; j-=8) {
            int c = getc(fp);
            word = Bitpack_newu(word, 8, j, c);
        }
        segment[i] = (uint32_t)word;
    }
    
    
    int *x = Seq_addlo(obj, segment);
    (void)x;


    return obj;
}

void mapSegment(Memory memory, int hint)
{
    

    //TODO: Check if reuasable elements are available
    
    
    uint32_t *segment = malloc(sizeof(uint32_t) * hint);
    for (int i = 0; i < hint; i++) {
        segment[i] = 0;
    }
    
    Seq_addhi(memory->segments, segment);
    
    // TODO: map bit pattern of non-zeros?

}


void unmapSegment(Memory memory, uint32_t id)
{
    // TODO: Error checking and bounds
    uint32_t *segment = (uint32_t*)Seq_get(memory->segments, id);
    free(segment);
    Seq_put(memory->segments, id, NULL);
    Seq_addlo(memory->reusable, (void*)(uintptr_t)id);
    
    
}



// int main(int argc, char *argv[])
// {
//     (void)argc;
// 
//     struct stat stats;
//     stat(argv[1], &stats);
//     int num_instructions = stats.st_size / 4;
// 
//     FILE *fp = fopen(argv[1], "rb");
// 
//     Memory object = Memory_new(fp, num_instructions);
// 
//     print_instructions(object, num_instructions);
//     unmapSegment(object, 0);
// 
//     mapSegment(object, 10);
// 
//     Memory_free(object);
// 
// 
// 
//     return 0;
// }








