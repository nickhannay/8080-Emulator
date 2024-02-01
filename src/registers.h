#ifndef _REGISTER_H
#define _REGISTER_H

#include "shared.h"
#include "8080-Emulator.h"
#include <stdint.h>

typedef struct RegisterPair{
    byte* high;
    byte* low;
    uint16_t* sp;
} RegisterPair;



RegisterPair* createRegPair();
void deleteRegPair(RegisterPair* rp);
RegisterPair* extractRegPair(State8080* p_state, byte opcode);
byte* extractReg(State8080* p_state, byte opcode);


#endif