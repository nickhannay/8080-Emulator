#ifndef _CPU_H
#define _CPU_H

#include "data_types.h"
#include "devices.h"
#include "memory.h"
#include <stdlib.h>

struct ConditionCodes{
    byte    flag_z:1;
    byte    flag_s:1;
    byte    flag_p:1;
    byte    flag_cy:1;
    byte    flag_ac:1;
    byte    pad:3;
};

typedef struct CPUState {
    byte    reg_a;
    byte    reg_b;
    byte    reg_c;
    byte    reg_d;
    byte    reg_e;
    byte    reg_h;
    byte    reg_l;
    uint16_t    sp;
    uint16_t    pc;
    Memory  memory;
    struct      ConditionCodes cc;
    byte     int_enable;
} CPUState;



byte getParity(byte input);

void setFlags(struct ConditionCodes* cc,  byte* reg);

CPUState* cpu_init();

int cpu_cleanup(CPUState* p_state);

byte cpu_fetch(CPUState* p_state);

int cpu_execute(CPUState* p_state, byte opcode, Device* devices);

#endif