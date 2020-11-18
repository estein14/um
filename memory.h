#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>
#include <stdlib.h>
#include "assert.h"
#include <seq.h>

Memory Memory_new(FILE *fp, int num_instructions);
Seq_T readFile(File *fp, int num_instructions);






#endif
