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


int op_LDAX(State8080* p_state, byte opcode){
    RegisterPair* rp = extractRegPair(p_state, opcode);

    p_state -> reg_a = p_state -> memory[u8_to_u16(*rp->high, *rp -> low)];

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


int op_ADI(State8080* p_state, byte opcode){
    uint16_t immediate = p_state -> memory[p_state -> pc];
    uint16_t acc = p_state -> reg_a;

    uint16_t res = acc + immediate;
    p_state -> reg_a = (byte) res;

    if(res >> 8 == 1){
        p_state -> cc.flag_cy = 1;
    }

    setFlags(&p_state->cc, &p_state -> reg_a);


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
    p_state -> cc.flag_cy |= ((result & 0x10000) == 0x10000);


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
    RegisterPair* rp = extractRegPair(p_state, opcode);
    uint16_t combined = u8_to_u16(*rp -> high, *rp -> low);
    combined += 1;
    *rp -> high = (combined & 0xff00 ) >> 8;
    *rp -> low = combined & 0x00ff;


    deleteRegPair(rp);
    return 0;
}

int op_STA(State8080* p_state, byte opcode){
    byte low = p_state -> memory[p_state ->  pc];
    byte high = p_state -> memory[p_state -> pc + 1];

    uint16_t addr = u8_to_u16(high, low);
    p_state -> memory[addr] = p_state -> reg_a;
    return 0;
}


int op_LDA(State8080* p_state, byte opcode){
    byte low = p_state -> memory[p_state ->  pc];
    byte high = p_state -> memory[p_state -> pc + 1];

    uint16_t addr = u8_to_u16(high, low);
    p_state -> reg_a = p_state -> memory[addr];
    return 0;
}

int op_MOV(State8080* p_state, byte opcode){
    byte* reg_dst  = extractReg(p_state, opcode);
    byte* reg_src = extractReg(p_state, opcode << 3);

    *reg_dst = *reg_src;
    return 0;
}


int op_ANA(State8080* p_state, byte opcode){

    byte* src_reg = extractReg(p_state, opcode << 3);
    p_state -> reg_a &= *src_reg;
    p_state -> cc.flag_ac = 0x00;

    setFlags(&p_state ->cc, &p_state -> reg_a);

    return 0;
}

int op_XRA(State8080* p_state, byte opcode){

    byte* src_reg = extractReg(p_state, opcode << 3);
    p_state -> reg_a ^= *src_reg;
    p_state -> cc.flag_ac = 0x00;

    setFlags(&p_state ->cc, &p_state -> reg_a);

    return 0;
}


int op_PUSH(State8080* p_state, byte opcode){
    RegisterPair* rp = extractRegPair(p_state, opcode);

    p_state -> memory[p_state -> sp - 2] = *rp->low;
    p_state -> memory[p_state -> sp - 1] = *rp -> high;

    p_state -> sp -= 2;

    deleteRegPair(rp);

    return 0;
}


int op_POP(State8080* p_state, byte opcode){

    RegisterPair* rp = extractRegPair(p_state, opcode);
    *rp -> low = p_state -> memory[p_state -> sp];
    *rp -> high = p_state -> memory[p_state -> sp + 1];
    p_state -> sp += 2;

    deleteRegPair(rp);
    return 0;
}


int op_JNZ(State8080* p_state, byte opcode){
    if(p_state -> cc.flag_z == 0){
        // jump
        uint16_t addr = u8_to_u16(p_state->memory[p_state->pc + 1], p_state->memory[p_state->pc]);
        // update pc
        p_state -> pc = addr;
    }
    else{
        p_state -> pc += 2;
    }

    return 0;
}


int op_JMP(State8080* p_state, byte opcode){
    uint16_t addr = u8_to_u16(p_state->memory[p_state->pc + 1], p_state->memory[p_state->pc]);
        // update pc
    p_state -> pc = addr;

    return 0;
}



int op_RET(State8080* p_state, byte opcode){
    uint16_t addr = u8_to_u16(p_state -> memory[p_state -> sp  + 1], p_state -> memory[p_state -> sp]);
    p_state -> sp += 2;

    p_state -> pc = addr;


    return 0;
}




int op_CALL(State8080* p_state, byte opcode){
    // CALL addr
    uint16_t addr = u8_to_u16(p_state -> memory[p_state -> pc + 1], p_state -> memory[p_state -> pc] );
    p_state -> pc += 2;

    byte low = p_state -> pc & 0x00ff;
    byte high = (p_state -> pc & 0xff00) >> 8;

    // push pc onto stack
    p_state -> memory[p_state -> sp - 1] = high;
    p_state -> memory[p_state -> sp - 2] = low;
    p_state -> sp -= 2;

    
    p_state -> pc = addr;

    return 0;
}



int op_OUT(State8080* p_state, byte opcode){


    return 0;
}















void setFlags(struct ConditionCodes* cc,  byte* reg){
    cc -> flag_z = *reg == 0;
    cc -> flag_p = getParity(*reg) == 0;
    cc -> flag_s = (*reg >> 7) & 1;
}



