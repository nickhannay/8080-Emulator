#include "8080-Emulator.h"
#include "operations.h"
#include "registers.h"
#include "shared.h"
#include <stdlib.h>


/* ********************************************* OPERATIONS ***************************************************** */

int op_LXI(State8080* p_state, byte opcode){
    RegisterPair* rp = extractRegPair(p_state, opcode);
    if(!rp -> sp){
        *rp->high = p_state -> memory[p_state -> pc + 1];
        *rp->low = p_state -> memory[p_state -> pc];
    }
    else{
        *rp -> sp = u8_to_u16(p_state -> memory[p_state -> pc + 1], p_state -> memory[p_state -> pc]);
    }
    
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
    setFlags(&p_state->cc, r );

    return 0;
}


int op_MVI(State8080* p_state, byte opcode){
    byte* r = extractReg(p_state, opcode); 
    *r = p_state -> memory[p_state -> pc];
    return 0;
}


int op_DAD(State8080* p_state, byte opcode){
    RegisterPair* rp = extractRegPair(p_state, opcode);

    uint32_t src = 0;
    if(!rp->sp){
        src = u8_to_u16(*rp->high, *rp->low);
    }
    else{
        src = *rp -> sp;
    }

    uint32_t result =  src + (uint32_t) u8_to_u16(p_state -> reg_h, p_state -> reg_l);
    

    // check overflow
    p_state -> cc.flag_cy |= (result & 0x10000) == 0x10000;


    p_state -> reg_h = (result & 0xff00 ) >> 8;
    p_state -> reg_l = result & 0x00ff;


    deleteRegPair(rp);
    return 0;

}



int op_RAC(State8080* p_state, byte opcode){
    byte op = opcode & 0x18;
    switch(op){
        case 0x00:
            // RLC
            break;
        case 0x01:
            // RRC
            byte LSB = p_state -> reg_a & 0x01;
            p_state -> reg_a >>= 1;
            p_state -> reg_a |= (LSB << 7);
            p_state -> cc.flag_cy &= LSB;
            break;
        case 0x10:
            // RAL
            break;
        case 0x11:
            // RAR
            break;
    }

    return 0;
}


int op_INX(State8080* p_state, byte opcode){
    

    
    return 0;
}





void setFlags(struct ConditionCodes* cc,  byte* reg){
    cc -> flag_z = *reg == 0;
    cc -> flag_p = getParity(*reg) == 0;
    cc -> flag_s = *reg & 0x80;
}



