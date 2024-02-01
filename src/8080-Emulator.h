#ifndef _8080_EMULATOR_H
#define _8080_EMULATOR_H

#include "shared.h"


struct ConditionCodes{
    byte    flag_z:1;
    byte    flag_s:1;
    byte    flag_p:1;
    byte    flag_cy:1;
    byte    flag_ac:1;
    byte    pad:3;
};

typedef struct State8080 {
    byte    reg_a;
    byte    reg_b;
    byte    reg_c;
    byte    reg_d;
    byte    reg_e;
    byte    reg_h;
    byte    reg_l;
    uint16_t    sp;
    uint16_t    pc;
    byte     *memory;
    struct      ConditionCodes cc;
    byte     int_enable;
} State8080;



int executeOp(State8080* state, byte opcode);
int emulate8080(State8080* state);




#endif