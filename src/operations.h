#ifndef _8080_EMULATOR_OPS_H
#define _8080_EMULATOR_OPS_H

#include <stdint.h>
#include "8080-Emulator.h"

byte getParity(byte input);
byte* extractReg(State8080* p_state, byte opcode);


void setDCRFlags(struct ConditionCodes* cc,  byte* reg);

int op_LXI(State8080* p_state, byte rp);

int op_STAX(State8080* p_state, byte rp);



#endif