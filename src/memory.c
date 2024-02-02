#include <stdlib.h>
#include "memory.h"

const size_t MEM_SIZE = 65536;

Memory memory_init(){
    return (Memory) calloc(MEM_SIZE, sizeof(byte));
}

int memory_cleanup(Memory mem){
    free(mem);

    return 0;
}