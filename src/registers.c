#include "registers.h"
#include <stdlib.h>

RegisterPair* createRegPair(){
    return ((RegisterPair*) calloc(1, sizeof(RegisterPair)));
}

void deleteRegPair(RegisterPair* rp){
    free(rp);
    return;
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