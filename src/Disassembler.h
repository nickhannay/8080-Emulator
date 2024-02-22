#ifndef _DISASSEMBLER_H
#define _DISASSEMBLER_H
#include "data_types.h"

int printOpCode(byte codes[], int index, uint64_t int_num);


int processBuffer(byte codes[], int buffer_size);




#endif