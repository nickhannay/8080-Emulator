#ifndef _8080_EMULATOR_H
#define _8080_EMULATOR_H

#include <stdint.h>
#include <stdlib.h>


typedef uint8_t byte;

#define u8_to_u16(high, low) (((uint16_t) high << 8 ) | low )


typedef struct RegisterPair{
    byte* high;
    byte* low;
} RegisterPair;

RegisterPair* createRegPair(){
    return ((RegisterPair*) calloc(1, sizeof(RegisterPair)));
}

void deleteRegPair(RegisterPair* rp){
    free(rp);
    return;
}




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