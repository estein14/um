/*
    memory.c
    
    Written by: Emmett Stein (estein14), Noah Wright (nwrigh05)
    
    Purpose: Holds all of the functions that interact with our main memory.
 */
#include "memory.h"

/*
    Arguments: File pointer and int num_instructions
    Prupose: Instantaites a new memory object that holds the a 
             fully populated 0th segment with instructions read 
             in from the inputed file. 
    Return: A memory struct object. 
    Invariants: None
 */
Memory Memory_new(FILE *fp, int num_instructions)
{
    assert(fp != NULL);
    Memory main = malloc(sizeof(*main));
    assert(main != NULL);

    main->seg      = readFile(fp, num_instructions);
    main->reusable = Seq_new(0);
    main->pSize    = num_instructions;
    main->ID       = 0;

    return main;
}

/*
    Arguments: Main memory structure
    Prupose: frees the entire memory structure including everything it holds. 
    Return: void
    Invariants: memory is not NULL
 */
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
    Arguments: opened file pointer and int num_instructions
    Prupose: Helper function for Memory_new. Instantiates a new 
             segment struct and populates the 0th segmenet with 
             the instructions read in from the file. 
    Return: a sequence object with a populated 0th segment
    Invariants: none
 */
Seq_T readFile(FILE *fp, int num_instructions)
{
    Seq_T obj = Seq_new(0);
    Seg seg   = malloc(sizeof(*seg));
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

/*
    Arguments: Main memory structure and int hint (size)
    Prupose: creates a new segment object and instantiates 
             a new array of the specified size. If there are 
             reusable segments already in the memory sequence
             the new segement will be placed there. Otherewise
             it will be added to the hi end of the segment. 
    Return: uint32_t ID of the segment. 
    Invariants: none
 */
uint32_t mapSegment(Memory memory, int hint)
{
    uint32_t id;
    uint32_t *array = calloc(hint, sizeof(uint32_t));
    Seg seg         = malloc(sizeof(*seg));
    assert(seg != NULL);
    assert(array != NULL);
    
    for (int i = 0; i < hint; i++) {
        array[i] = 0;
    }
    
    seg->length = hint;
    seg->array  = array;

    if (Seq_length(memory->reusable) != 0) {
        id = (uint32_t)(uintptr_t)Seq_remhi(memory->reusable);
        Seq_put(memory->seg, id, seg);
    }
    else {
        Seq_addhi(memory->seg, seg);
        memory->ID++;
        id = memory->ID;
    }
    return id;
}

/*
    Arguments: Main memory structure and id for specific segment
    Prupose: unmaps the segment specified by the id number and 
             frees the contents, replacing the segment with NULL.
    Return: void
    Invariants: none
 */
void unmapSegment(Memory memory, uint32_t id)
{
    assert(id != 0);
    Seg seg         = Seq_get(memory->seg, id);
    uint32_t *array = seg->array;
    free(array);
    free(seg);
    
    Seq_put(memory->seg, id, NULL);
    Seq_addhi(memory->reusable, (void*)(uintptr_t)id);
}

/*
    Arguments: Main memory structure and id for specific segment
    Prupose: returns the length of the segment at the index id
    Return: uint32_t length of segment
    Invariants: none
 */
uint32_t segLength(Memory memory, uint32_t id)
{
    Seg seg         = Seq_get(memory->seg, id);
    uint32_t length = seg->length;
    return length;
}
