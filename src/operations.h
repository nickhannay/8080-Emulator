#ifndef _8080_EMULATOR_OPS_H
#define _8080_EMULATOR_OPS_H

#include <stdint.h>
#include "8080-Emulator.h"

byte getParity(byte input);

void setDCRFlags(struct ConditionCodes* cc,  byte* reg);

int op_LXI(State8080* p_state, byte rp);

int op_STAX(State8080* p_state, byte rp);

int op_DCR(State8080* p_state, byte opcode);

int op_DAD(State8080* p_state, byte opcode);

int op_MVI(State8080* p_state, byte opcode);

int op_DAD(State8080* p_state, byte opcode);


#endif