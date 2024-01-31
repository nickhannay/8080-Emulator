#include "operations.h"
#include "8080-Emulator.h"
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


void setDCRFlags(struct ConditionCodes* cc,  byte* reg){
    cc -> flag_z = *reg == 0;
    cc -> flag_p = getParity(*reg) == 0;
    cc -> flag_s = *reg & 0x80;
}


/* **************************************************** UTILITY FUNCTIONS **************************************************************/

// returns 0 on even and 1 on odd
byte getParity(byte input){
    byte ones = 0;
    for(int i = 0; i < 8 ; i++){
        ones += (input >> i ) & 1;
    }

    return ones & 1;
}

RegisterPair* extractRegPair(State8080* p_state, byte opcode){
    byte regCode = (opcode & 0x30) >> 4;
    RegisterPair* rp = createRegPair();
    switch(regCode){
        case 0b00:
            rp -> high = &p_state -> reg_b;
            rp -> low = &p_state -> reg_c;
            break;
        case 0b01: 
            rp -> high = &p_state -> reg_d;
            rp -> low = &p_state -> reg_e;
            break;

        case 0b10:
            rp -> high = &p_state -> reg_h;
            rp -> low = &p_state -> reg_l;
            break;

        case 0b11:
            rp -> high = &p_state -> reg_b;
            rp -> low = &p_state -> reg_c;
            break;
    }

    return rp;
}



byte* extractReg(State8080* p_state, byte opcode){
    byte regCode = (opcode & 0x38) >> 3;
    byte *reg = NULL;
    switch(regCode){
        case 0b000:
            reg = p_state -> reg_b;
            break;
        case 0b001: 
            reg = p_state -> reg_c;
            break;

        case 0b010:
            reg = p_state -> reg_d;
            break;

        case 0b011:
            reg = p_state -> reg_e;
            break;

        case 0b100:
            reg = p_state -> reg_h;
            break;

        case 0b101:
            reg = p_state -> reg_l;
            break;

        case 0b110:
            // setDCRFlags( &p_state->cc, &p_state->reg_m );
            // TODO : Memory access
            break;
        case 0b111:
            reg = p_state -> reg_a;
            break;
    }

    return reg;
}