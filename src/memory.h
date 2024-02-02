#ifndef _MEMORY_H
#define _MEMORY_H

#include <stdlib.h>
#include "data_types.h"


extern const size_t MEM_SIZE;

typedef byte* Memory;

Memory memory_init();

int memory_cleanup(Memory mem);


#endif