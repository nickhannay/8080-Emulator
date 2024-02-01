#ifndef _8080_EMULATOR_OPS_H
#define _8080_EMULATOR_OPS_H

#include <stdint.h>
#include "8080-Emulator.h"


byte getParity(byte input);
void setFlags(struct ConditionCodes* cc,  byte* reg);




/* ****************************  DIRECT ADDRESSING INSTRUCTIONS ******************************* */
int op_STA(State8080* p_state, byte opcode);

int op_LDA(State8080* p_state, byte opcode);






/* ****************************  IMMEDIATE INSTRUCTIONS ******************************* */
int op_MVI(State8080* p_state, byte opcode);

int op_LXI(State8080* p_state, byte opcode);




/* ****************************  REGISTER PAIR INSTRUCTIONS ******************************* */
int op_INX(State8080* p_state, byte opcode);

int op_DAD(State8080* p_state, byte opcode);




/* ****************************  ROTATE ACCUMULATOR INSTRUCTIONS ******************************* */
int op_RAC(State8080* p_state, byte opcode);




/* *******************  REGISTER OR MEMORY TO ACCUMULATOR INSTRUCTIONS ****************** */
int op_ANA(State8080* p_state, byte opcode);




/* ****************************  DATA TRANSFER INSTRUCTIONS ******************************* */
int op_MOV(State8080* p_state, byte opcode);

int op_STAX(State8080* p_state, byte opcode);

int op_LDAX(State8080* p_state, byte opcode);




/* ****************************  SINGLE REGISTER INSTRUCTIONS *************************** */
int op_DCR(State8080* p_state, byte opcode);







#endif