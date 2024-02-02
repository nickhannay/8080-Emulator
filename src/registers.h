#ifndef _REGISTER_H
#define _REGISTER_H

#include <stdlib.h>
#include <stdint.h>
#include "data_types.h"
#include "cpu.h"

typedef struct RegisterPair{
    byte* high;
    byte* low;
    uint16_t* sp;
} RegisterPair;



RegisterPair* createRegPair();

void deleteRegPair(RegisterPair* rp);

RegisterPair* extractRegPair(CPUState* p_state, byte opcode);

byte* extractReg(CPUState* p_state, byte opcode);


#endif