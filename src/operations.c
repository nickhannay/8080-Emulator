#include "8080-Emulator.h"
#include "operations.h"
#include "registers.h"
#include "shared.h"
#include <stdlib.h>


/* ********************************************* OPERATIONS ***************************************************** */

int op_LXI(State8080* p_state, byte opcode){
    RegisterPair* rp = extractRegPair(p_state, opcode);
    *rp->high = p_state -> memory[p_state -> pc + 1];
    *rp->low = p_state -> memory[p_state -> pc];

    deleteRegPair(rp);
    return 0; 
}


int op_STAX(State8080* p_state, byte opcode){
    RegisterPair* rp = extractRegPair(p_state, opcode);

    p_state -> memory[u8_to_u16(*rp->high, *rp -> low)] = p_state -> reg_a;
    deleteRegPair(rp);
    return 0;
}


int op_DCR(State8080* p_state, byte opcode){
    byte* r = extractReg(p_state, opcode);
    *r -= 1;
    setDCRFlags(&p_state->cc, r );

    return 0;
}


int op_MVI(State8080* p_state, byte opcode){
    byte* r = extractReg(p_state, opcode); 
    *r = p_state -> memory[p_state -> pc];
    return 0;
}


int op_DAD(State8080* p_state, byte opcode){

}



void setDCRFlags(struct ConditionCodes* cc,  byte* reg){
    cc -> flag_z = *reg == 0;
    cc -> flag_p = getParity(*reg) == 0;
    cc -> flag_s = *reg & 0x80;
}



