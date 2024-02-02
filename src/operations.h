#ifndef _8080_EMULATOR_OPS_H
#define _8080_EMULATOR_OPS_H

#include "data_types.h"
#include "cpu.h"


/* ****************************  JUMP INSTRUCTIONS ******************************* */
int op_JNZ(CPUState* p_state, byte opcode);

int op_JMP(CPUState* p_state, byte opcode);




/* ****************************  DIRECT ADDRESSING INSTRUCTIONS ******************************* */
int op_STA(CPUState* p_state, byte opcode);

int op_LDA(CPUState* p_state, byte opcode);






/* ****************************  IMMEDIATE INSTRUCTIONS ******************************* */
int op_MVI(CPUState* p_state, byte opcode);

int op_LXI(CPUState* p_state, byte opcode);

int op_ADI(CPUState* p_state, byte opcode);

int op_ANI(CPUState* p_state, byte opcode);

int op_CPI(CPUState* p_state, byte opcode);



/* ****************************  REGISTER PAIR INSTRUCTIONS ******************************* */
int op_INX(CPUState* p_state, byte opcode);

int op_DAD(CPUState* p_state, byte opcode);

int op_PUSH(CPUState* p_state, byte opcode);

int op_POP(CPUState* p_state, byte opcode);

int op_XCHG(CPUState* p_state, byte opcode);




/* ****************************  ROTATE ACCUMULATOR INSTRUCTIONS ******************************* */
int op_RAC(CPUState* p_state, byte opcode);




/* *******************  REGISTER OR MEMORY TO ACCUMULATOR INSTRUCTIONS ****************** */
int op_ANA(CPUState* p_state, byte opcode);

int op_XRA(CPUState* p_state, byte opcode);




/* ****************************  DATA TRANSFER INSTRUCTIONS ******************************* */
int op_MOV(CPUState* p_state, byte opcode);

int op_STAX(CPUState* p_state, byte opcode);

int op_LDAX(CPUState* p_state, byte opcode);




/* ****************************  SINGLE REGISTER INSTRUCTIONS *************************** */
int op_DCR(CPUState* p_state, byte opcode);





/* ****************************  RETURN FROM SUBROUTINE INSTRUCTIONS *************************** */
int op_RET(CPUState* p_state, byte opcode);






/* ****************************  CALL SUBROUTINE INSTRUCTIONS *************************** */
int op_CALL(CPUState* p_state, byte opcode);





/* ****************************  IO INSTRUCTIONS *************************** */
int op_OUT(CPUState* p_state, byte opcode);





//
int op_setI(CPUState* p_state, byte toggle);


#endif