#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "operations.h"
#include "registers.h"



int op_LXI(CPUState* p_state, byte opcode){
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


int op_LDAX(CPUState* p_state, byte opcode){
    RegisterPair* rp = extractRegPair(p_state, opcode);

    p_state -> reg_a = p_state -> memory[u8_to_u16(*rp->high, *rp -> low)];

    deleteRegPair(rp);
    return 0;
}


int op_STAX(CPUState* p_state, byte opcode){
    RegisterPair* rp = extractRegPair(p_state, opcode);
    
    p_state -> memory[u8_to_u16(*rp->high, *rp -> low)] = p_state -> reg_a;
    deleteRegPair(rp);
    return 0;
}


int op_DCR(CPUState* p_state, byte opcode){
    byte* r = extractReg(p_state, opcode);
    byte tmp = *r;
    *r -= 1;
    printf("Before: %02x --- After: %02x\n",tmp, *r );
    setFlags(&p_state->cc, r );

    return 0;
}


int op_MVI(CPUState* p_state, byte opcode){
    byte* r = extractReg(p_state, opcode); 
    *r = p_state -> memory[p_state -> pc];
    return 0;
}


int op_ADI(CPUState* p_state, byte opcode){
    uint16_t immediate = p_state -> memory[p_state -> pc];
    uint16_t acc = p_state -> reg_a;

    uint16_t res = acc + immediate;
    p_state -> reg_a = (byte) res;

    if((res >> 8) == 1){
        p_state -> cc.flag_cy = 1;
    }

    setFlags(&p_state->cc, &p_state -> reg_a);


    return 0;
}


int op_CPI(CPUState* p_state, byte opcode){
    uint16_t immediate = p_state -> memory[p_state -> pc];

    uint16_t twos_comp  = ~immediate + 1;

    uint16_t cmp = p_state -> reg_a + twos_comp;

    p_state -> cc.flag_cy = (cmp & 0xf0) == 0xf0;

    byte res = (byte) cmp;
    setFlags(&p_state -> cc, &res);


    return 0;
}



int op_ANI(CPUState* p_state, byte opcode){

    p_state -> reg_a &= p_state -> memory[p_state -> pc];

    p_state -> cc.flag_cy = 0;

    setFlags(&p_state->cc, &p_state->reg_a);

    return 0;
}



int op_DAD(CPUState* p_state, byte opcode){
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


int op_XCHG(CPUState* p_state, byte opcode){
    // exchange high order byte
    byte tmp = p_state -> reg_d;
    p_state -> reg_d = p_state -> reg_h;
    p_state -> reg_h = tmp;

    // exchange low order byte
    tmp = p_state -> reg_e;
    p_state -> reg_e = p_state -> reg_l;
    p_state -> reg_l = tmp; 

    return 0;
}



int op_RAC(CPUState* p_state, byte opcode){
    byte op = opcode >> 3 & 0x03;
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


int op_INX(CPUState* p_state, byte opcode){
    RegisterPair* rp = extractRegPair(p_state, opcode);
    uint16_t combined = u8_to_u16(*rp -> high, *rp -> low);
    combined += 1;
    *rp -> high = (combined & 0xff00 ) >> 8;
    *rp -> low = combined & 0x00ff;


    deleteRegPair(rp);
    return 0;
}

int op_STA(CPUState* p_state, byte opcode){
    byte low = p_state -> memory[p_state ->  pc];
    byte high = p_state -> memory[p_state -> pc + 1];

    uint16_t addr = u8_to_u16(high, low);
    p_state -> memory[addr] = p_state -> reg_a;
    return 0;
}


int op_LDA(CPUState* p_state, byte opcode){
    byte low = p_state -> memory[p_state ->  pc];
    byte high = p_state -> memory[p_state -> pc + 1];

    uint16_t addr = u8_to_u16(high, low);
    p_state -> reg_a = p_state -> memory[addr];
    return 0;
}

int op_MOV(CPUState* p_state, byte opcode){
    byte* reg_dst  = extractReg(p_state, opcode);
    byte* reg_src = extractReg(p_state, opcode << 3);

    *reg_dst = *reg_src;
    return 0;
}


int op_ANA(CPUState* p_state, byte opcode){

    byte* src_reg = extractReg(p_state, opcode << 3);
    p_state -> reg_a &= *src_reg;
    p_state -> cc.flag_ac = 0x00;

    setFlags(&p_state ->cc, &p_state -> reg_a);

    return 0;
}

int op_XRA(CPUState* p_state, byte opcode){

    byte* src_reg = extractReg(p_state, opcode << 3);
    p_state -> reg_a ^= *src_reg;
    p_state -> cc.flag_ac = 0x00;

    setFlags(&p_state ->cc, &p_state -> reg_a);

    return 0;
}


int op_PUSH(CPUState* p_state, byte opcode){
    if((opcode & 0x30) >> 4 == 0x03){
        // handle PSW 
        p_state -> memory[p_state -> sp - 1]  = p_state -> reg_a;

        // FLAGs
        byte flags = p_state -> cc.flag_cy | 
                     0x02 |
                     p_state -> cc.flag_p << 2 |
                     0x00 |
                     p_state -> cc.flag_ac << 4 |
                     0x00 |
                     p_state -> cc.flag_z << 6 | 
                     p_state -> cc.flag_s << 7;

        p_state -> memory[p_state -> sp - 2] = flags;
        printf("pushed: %04x onto stack\n", u8_to_u16(p_state -> memory [p_state -> sp - 1],p_state -> memory[p_state -> sp - 2]));

    }
    else {
        RegisterPair* rp = extractRegPair(p_state, opcode);
        p_state -> memory[p_state -> sp - 2] = *rp->low;
        p_state -> memory[p_state -> sp - 1] = *rp -> high;
        deleteRegPair(rp);
        printf("pushed: %04x onto stack\n", u8_to_u16(p_state -> memory [p_state -> sp - 1],p_state -> memory[p_state -> sp - 2]));
    }

    p_state -> sp -= 2;

    return 0;
}


int op_POP(CPUState* p_state, byte opcode){
    if((opcode & 0x30) >> 4 == 0x03){
        // handle PSW 
        byte flags = p_state -> memory[p_state -> sp];
        p_state -> reg_a = p_state -> memory[p_state -> sp + 1];
        printf("popped: %04x off of stack\n", u8_to_u16(p_state -> memory [p_state -> sp + 1],p_state -> memory[p_state -> sp]));

        p_state -> cc.flag_ac = flags >> 4 & 1;
        p_state -> cc.flag_cy = flags & 1;
        p_state -> cc.flag_z = flags >> 6 & 1;
        p_state -> cc.flag_s = flags >> 7 & 1;
        p_state -> cc.flag_p = flags >> 2 & 1;
    }
    else{
        RegisterPair* rp = extractRegPair(p_state, opcode);
        *rp -> low = p_state -> memory[p_state -> sp];
        *rp -> high = p_state -> memory[p_state -> sp + 1];

        printf("popped: %04x off of stack\n", u8_to_u16(p_state -> memory [p_state -> sp + 1],p_state -> memory[p_state -> sp]));
        deleteRegPair(rp);
    }

    p_state -> sp += 2;
    return 0;
}


int op_JNZ(CPUState* p_state, byte opcode){
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


int op_JMP(CPUState* p_state, byte opcode){
    uint16_t addr = u8_to_u16(p_state->memory[p_state->pc + 1], p_state->memory[p_state->pc]);
        // update pc
    p_state -> pc = addr;

    return 0;
}



int op_RET(CPUState* p_state, byte opcode){
    uint16_t addr = u8_to_u16(p_state -> memory[p_state -> sp  + 1], p_state -> memory[p_state -> sp]);

    printf("Returning to %04x\n", addr);
    p_state -> sp += 2;

    p_state -> pc = addr;

    return 0;
}




int op_CALL(CPUState* p_state, byte opcode){
    // CALL addr
    uint16_t addr = u8_to_u16(p_state -> memory[p_state -> pc + 1], p_state -> memory[p_state -> pc] );
    p_state -> pc += 2;
    
    

    byte low = p_state -> pc & 0x00ff;
    byte high = (p_state -> pc & 0xff00) >> 8;

    printf("calling %04x\n", u8_to_u16(high, low));

    // push pc onto stack
    p_state -> memory[p_state -> sp - 1] = high;
    p_state -> memory[p_state -> sp - 2] = low;
    p_state -> sp -= 2;

    
    p_state -> pc = addr;

    return 0;
}




int op_setI(CPUState* p_state, byte toggle){
    p_state -> int_enable = toggle;

    return 0;
}



int op_OUT(CPUState* p_state, Device* devices){
    byte device_id = p_state -> memory[p_state -> pc];
    devices_write(device_id, p_state -> reg_a, devices);

    return 0;
}


int op_IN(CPUState* p_state, Device* devices){
    byte device_id = p_state -> memory[p_state -> pc];
    devices_read(device_id, &p_state -> reg_a, devices);

    return 0;
}




void op_unimplemented(byte opcode){
    perror("INSTRUCTION %02x has not been implemented\n", opcode);
    exit(EXIT_FAILURE);
}








