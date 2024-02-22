#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "operations.h"
#include "registers.h"


/* ****************************  CARRY BIT INSTRUCTIONS *************************** */

/*
    Complement Carry

    If the Carry bit = 0, it is set to 1. If the Carry bit = 1, it is reset to O

    Condtion bits affected: Carry
*/
int op_CMC(CPUState* p_state){
    p_state -> cc.flag_cy = ~p_state -> cc.flag_cy;

    return CYCLES(4);
}

/*
    Set Carry

    The carry bit is set to 1.

    Condtion bits affected: Carry
*/
int op_STC(CPUState* p_state){
    p_state -> cc.flag_cy = 1;

    return CYCLES(4);
}

















/* ****************************  SINGLE REGISTER INSTRUCTIONS *************************** */

/*
    Increment Register or Memory

    The specified register or memory byte is incremented by one.

    Condtion bits affected: Zero, Sign, Parity, Aux Carry
*/
int op_INR(CPUState* p_state, byte opcode){
    byte *reg = extractReg(p_state, opcode);

    byte nib1 = *reg & 0x0f;


    *reg += 1;


    if(((nib1 + 1) & 0x10) == 0x10){
        p_state -> cc.flag_ac = 1;
    }
    else{
        p_state -> cc.flag_ac = 0;
    }

    cpu_setFlags(&p_state -> cc, reg); 

    if(cpu_checkMemOp(opcode)){
        return CYCLES(10);
    }
    else{
        return CYCLES(5);
    }

}

/*
    Decrement Register or Memory

    The specified register or memory byte is decremented by one.

    Condtion bits affected: Zero, Sign, Parity, Aux Carry
*/
int op_DCR(CPUState* p_state, byte opcode){
    byte* r = extractReg(p_state, opcode);

    byte low_nib = *r & 0x0f;



    *r -= 1;

    if(((low_nib - 1) & 0x10) == 0x10){
        p_state -> cc.flag_ac = 0;
    }
    else{
        p_state -> cc.flag_ac = 1;
    }
    cpu_setFlags(&p_state->cc, r );

    if(cpu_checkMemOp(opcode)){
        return CYCLES(10);
    }
    else{
        return CYCLES(5);
    }
}

/*
    Complement Accumulator

    Each bit of the contents of the accumulator is complemented (producing the one's complement)

    Condition bits affected: None
*/
int op_CMA(CPUState* p_state){

    p_state -> reg_a = ~p_state -> reg_a;

    return CYCLES(4);
}

/*
    Decimal Adjust Register

    The 8 bit hex number in the accumulator is adjusted to form two 4 digit BCD decimal digits

    Condition bits affected: All
*/
int op_DAA(CPUState* p_state){

    byte low_nibble = p_state -> reg_a & 0x0f;
    
    byte *acc = &p_state->reg_a;

    if(low_nibble > 9 || p_state -> cc.flag_ac == 1){
        if(((low_nibble + 6) & 0x10) == 0x10){
            p_state -> cc.flag_ac = 1;
        }
        else{
            p_state -> cc.flag_ac = 0;
        } 

        *acc += 6;
    }

    byte hi_nibble = (*acc & 0xf0) >> 4;
    if(hi_nibble > 9 || p_state -> cc.flag_cy == 1){
        if(((hi_nibble += 6) & 0x10) == 0x10){
            p_state -> cc.flag_cy = 1;
        }

        *acc &= 0x0f;
        *acc |= (hi_nibble << 4);

    }

    cpu_setFlags(&p_state -> cc, acc);

    return CYCLES(4);

}



















/* ****************************  DATA TRANSFER INSTRUCTIONS ******************************* */

/*
    MOV

    One byte of data is moved from the register specified by src (the source register) to the register specified by dst (the destination register). 
    The data replaces the contents of the destination register; the source remains unchanged

    Condition bits affected: None
*/
int op_MOV(CPUState* p_state, byte opcode){
    byte* reg_dst  = extractReg(p_state, opcode);
    byte* reg_src = extractReg(p_state, opcode << 3);
    *reg_dst = *reg_src;

    if(cpu_checkMemOp(opcode)){
        return CYCLES(7);
    }
    else {
        return CYCLES(5);
    }
}

/*
    Store Accumulator

    The contents of the accumulator are stored in the memory location addressed by registers Band C, or by registers 0 and E

    Condition bits affected: None
*/
int op_STAX(CPUState* p_state, byte opcode){
    RegisterPair* rp = extractRegPair(p_state, opcode);
    
    p_state -> memory[u8_to_u16(*rp->high, *rp -> low)] = p_state -> reg_a;
    deleteRegPair(rp);
    return CYCLES(7);
}

/*
    Load Accumulator

    The contents of the memory location addressed by registers Band C, or by registers 0 and E, replace the contents of the accumulator

    Condition bits affected: None
*/
int op_LDAX(CPUState* p_state, byte opcode){
    RegisterPair* rp = extractRegPair(p_state, opcode);

    p_state -> reg_a = p_state -> memory[u8_to_u16(*rp->high, *rp -> low)];

    deleteRegPair(rp);
    return CYCLES(7);
}















/* *******************  REGISTER OR MEMORY TO ACCUMULATOR INSTRUCTIONS ****************** */

/*
    Logical and register or Memory with Accumulator

    The specified byte is logically ANDed bit by bit with the contents of the accumulator.
     The Carry bit is reset to zero

    Condition bits affected: Carry, Zero, Sign, Parity
*/
int op_ANA(CPUState* p_state, byte opcode){

    byte* src_reg = extractReg(p_state, opcode << 3);
    p_state -> reg_a &= *src_reg;

    p_state -> cc.flag_ac = 0;
    p_state -> cc.flag_cy = 0;
    cpu_setFlags(&p_state ->cc, &p_state -> reg_a);

    return CYCLES(1);
}


/*
    Logical Exclusive-Or Register to Memory With Accumulator (0 Accumulator)

    The specified byte is EXCLUSIVE-ORed bit by bit with the contents of the accumulator. 
    The Carry bit is reset to zero.

    Condition bits affected: Carry, Sign, Zero, Parity, Aux Carry
*/
int op_XRA(CPUState* p_state, byte opcode){

    byte* src_reg = extractReg(p_state, opcode << 3);
    p_state -> reg_a ^= *src_reg;

    p_state -> cc.flag_cy = 0;
    p_state -> cc.flag_ac = 0;
    cpu_setFlags(&p_state ->cc, &p_state -> reg_a);

    if(cpu_checkMemOp(opcode)){
        return CYCLES(7);
    }
    else {
        return CYCLES(5);
    }
}


/*
    ADD Register or Memory To Accumulator

    The specified byte is added to the contents of the accumulator using two's complement arithmetic.

    Condtion bits affected: Carry, Zero, Sign, Parity, Aux Carry
*/
int op_ADD(CPUState* p_state, byte opcode){
    byte *src = extractReg(p_state, opcode << 3);
    byte *acc = &p_state -> reg_a;

    if(cpu_isAuxCarry(src, acc)){
        p_state -> cc.flag_ac = 1;
    } else{
        p_state -> cc.flag_ac = 0;
    }
    
    
    uint16_t res = cpu_add(src, acc);
    if((res >> 8) == 1){
        p_state -> cc.flag_cy = 1;
    }
    else{
        p_state -> cc.flag_cy = 0;
    }

    cpu_setFlags(&p_state-> cc, acc);

    if(cpu_checkMemOp(opcode)){
        return CYCLES(7);
    } else{
        return CYCLES(4);
    }  
    
}


/*
    ADD Register or Memory to Accumulator With Carry

    The specified byte plus the content of the Carry bit is added to the contents of the accumulator

    Condition bits affected: Carry, Sign, Zero, Parity, Aux Carry
*/
int op_ADC(CPUState* p_state, byte opcode){
    byte *src = extractReg(p_state, opcode << 3);
    byte *acc = &p_state -> reg_a;
    byte carry = p_state -> cc.flag_cy;
    byte aux_acc = *acc;

    // capture carry bit from: register/memory + accumulator + carry
    uint16_t add_res = cpu_add(src, acc);
    cpu_add(&carry, acc);
    uint16_t res =  add_res +  carry;

    // check for carry
    if((res >> 8) == 1){
        p_state -> cc.flag_cy = 1;
    }
    else{
        p_state -> cc.flag_cy = 0;
    }
     

    if(cpu_isAuxCarry(src, &aux_acc)){
        p_state -> cc.flag_ac = 1;
    }
    else{
        byte low_nib = add_res & 0x000f;
        if(cpu_isAuxCarry(&carry, &low_nib)){
            p_state -> cc.flag_ac = 1;
        }
        else{
            p_state -> cc.flag_ac = 0;
        }
    }

    cpu_setFlags(&p_state -> cc, acc);


    if(cpu_checkMemOp(opcode)){
        return CYCLES(7);
    } else{
        return CYCLES(4);
    }

}


/*
    Subtract Register or Memory from Accumulator

    The specified byte is subtracted from the accumulator using two's complement arithmetic

    Condition bits affected: Carry, Sign, Zero, Parity, Aux Carry
*/
int op_SUB(CPUState* p_state, byte opcode){
    byte *reg = extractReg(p_state, opcode << 3); 
    byte *acc = &p_state -> reg_a;
    byte twos_comp = (~*reg) + 1;

    if(cpu_isAuxCarry(&twos_comp, acc)){
        p_state -> cc.flag_ac = 1;
    } else{
        p_state -> cc.flag_ac = 0;
    }


    // no carry indicates presence of borrow, thus carry flag represents borrow
    uint16_t res = cpu_add(&twos_comp, acc);
    if((res & 0x0100) == 0x0100){
        p_state -> cc.flag_cy = 0;
    } else{
        p_state -> cc.flag_ac = 1;
    }


    cpu_setFlags(&p_state -> cc, acc);

    if(cpu_checkMemOp(opcode)){
        return CYCLES(7);
    } else{
        return CYCLES(4);
    }

}


/*
    Subtract Register or Memory from Accumulator with Borrow

    The Carry bit is internally added to the contents of the specified byte. 
    This value is then subtracted from the accumulator using two's complement arithmetic

    Condition bits affected: Carry, Zero, Sign, Parity, Aux Carry
*/
int op_SBB(CPUState* p_state, byte opcode){
    byte *reg = extractReg(p_state, opcode << 3); 
    byte *acc = &p_state -> reg_a;

    byte twos_comp = ~(*reg + p_state -> cc.flag_cy) + 1;

    if(cpu_isAuxCarry(&twos_comp, acc)){
        p_state -> cc.flag_ac = 1;
    } else{
        p_state -> cc.flag_ac = 0;
    }


    // no carry indicates presence of borrow, thus carry flag represents borrow
    uint16_t res = cpu_add(&twos_comp, acc);
    if((res & 0x0100) == 0x0100){
        p_state -> cc.flag_cy = 0;
    } else{
        p_state -> cc.flag_ac = 1;
    }


    cpu_setFlags(&p_state -> cc, acc);

    if(cpu_checkMemOp(opcode)){
        return CYCLES(7);
    } else{
        return CYCLES(4);
    }

}


/*
    Logical or Register or Memory With Accumulator

    The specified byte is logically ORed bit by bit with the contents of the accumulator. 
    The carry bit is reset to zero.

    Condition bits affected: Carry, Zero, Sign, Parity, Aux Carry
*/
int op_ORA(CPUState* p_state, byte opcode){
    byte *reg = extractReg(p_state, opcode << 3);
    p_state -> reg_a |=  *reg;

    p_state -> cc.flag_ac = 0;
    p_state -> cc.flag_cy = 0;

    cpu_setFlags(&p_state -> cc, &p_state -> reg_a);


    if(cpu_checkMemOp(opcode)){
        return CYCLES(7);
    } else{
        return CYCLES(4);
    }

}


/*
    Compare Register or Memory With Accumulator

    The specified byte is compared to the contents of the accumulator. 
    The comparison is performed by internally subtracting the contents of REG 
    from the accumulator (leaving both unchanged) and setting the condition bits according to the result

    Condition bits affected: Carry, Sign, Zero, Parity, Aux Carry
*/
int op_CMP(CPUState* p_state, byte opcode){
    // same as SUB, except Accumulator is left unchanged
    byte *reg = extractReg(p_state, opcode << 3); 
    byte *acc = &p_state -> reg_a;
    byte twos_comp = (~*reg) + 1;

    if(cpu_isAuxCarry(&twos_comp, acc)){
        p_state -> cc.flag_ac = 1;
    } else{
        p_state -> cc.flag_ac = 0;
    }


    // no carry indicates presence of borrow, thus carry flag represents borrow
    uint16_t res = (uint16_t) *acc + (uint16_t) twos_comp;
    if((*reg ^ p_state -> reg_a) & 0x80){
        // different sign
        if((res & 0x0100) == 0x0100){
            // overflow
            p_state -> cc.flag_cy = 1;
        } else{
            // no overflow
            p_state -> cc.flag_cy = 0;
        };
    } else{
        // same sign
        if((res & 0x0100) == 0x0100){
            // overflow -> no borrow
            p_state -> cc.flag_cy = 0;
        } else{
            // no overflow -> borrow
            p_state -> cc.flag_cy = 1;
        };
    }

    byte low_byte = res & 0x00ff;
    cpu_setFlags(&p_state -> cc, &low_byte);

    if(cpu_checkMemOp(opcode)){
        return CYCLES(7);
    } else{
        return CYCLES(4);
    }

}




/* ****************************  ROTATE ACCUMULATOR INSTRUCTIONS ******************************* */

int op_RAC(CPUState* p_state, byte opcode){
    byte op = opcode >> 3 & 0x03;
    byte MSB, LSB = 0;
    switch(op){
        case 0x00:
            // RLC
            MSB = p_state -> reg_a & 0x80;
            p_state -> cc.flag_cy = MSB;
            p_state -> reg_a <<= 1;
            p_state -> reg_a |= (MSB >> 7);

            break;
        case 0x01:
            // RRC
            LSB = p_state -> reg_a & 0x01;
            p_state -> cc.flag_cy = LSB;
            p_state -> reg_a >>= 1;
            p_state -> reg_a |= (LSB << 7);

            break;
        case 0x10:
            // RAL
            MSB = p_state -> reg_a & 0x80;
            p_state -> reg_a <<= 1;
            p_state -> reg_a |= p_state -> cc.flag_cy;
            p_state -> cc.flag_cy = MSB;
            
            break;
        case 0x11:
            // RAR
            LSB = p_state -> reg_a & 0x01;
            p_state -> reg_a >>= 1;
            p_state -> reg_a |= (p_state -> cc.flag_cy << 7);
            p_state -> cc.flag_cy = LSB;

            break;
    }

    return CYCLES(4);
}









/* ****************************  REGISTER PAIR INSTRUCTIONS ******************************* */

/*
    Double Add

    The 16-bit number in the specified register pair is added to the 16-bit number held in the H and L
    registers using two's complement arithmetic. The result replaces the contents of the Hand L registers

    Condtion bits affected: Carry
*/
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
    return CYCLES(10);

}


/*
    Exchange Registers

    The 16 bits of data held in the Hand L registers are exchanged with the 16 bits of data held in the
    D and E registers.

    Condition bits affected: None
*/
int op_XCHG(CPUState* p_state, byte opcode){
    // exchange high order byte
    byte tmp = p_state -> reg_d;
    p_state -> reg_d = p_state -> reg_h;
    p_state -> reg_h = tmp;

    // exchange low order byte
    tmp = p_state -> reg_e;
    p_state -> reg_e = p_state -> reg_l;
    p_state -> reg_l = tmp; 

    return CYCLES(4);
}


/*
    Increment Register Pair

    The 16-bit number held in the specified register pair is incremented by one

    Condition bits affected: None
*/
int op_INX(CPUState* p_state, byte opcode){
    RegisterPair* rp = extractRegPair(p_state, opcode);
    uint16_t combined = u8_to_u16(*rp -> high, *rp -> low);
    combined += 1;
    *rp -> high = (combined & 0xff00 ) >> 8;
    *rp -> low = combined & 0x00ff;


    deleteRegPair(rp);
    return CYCLES(5);
}


/*
    Decrement Register Pair

    The 16-bit number held in the specified register pair is decremented by one

    Condition bits affected: None
*/
int op_DCX(CPUState* p_state, byte opcode){
    RegisterPair* rp = extractRegPair(p_state, opcode);
    uint16_t combined = u8_to_u16(*rp -> high, *rp -> low);
    combined -= 1;
    *rp -> high = (combined & 0xff00 ) >> 8;
    *rp -> low = combined & 0x00ff;


    deleteRegPair(rp);

    return CYCLES(5);
}


/*
    Push Data Onto Stack

    The contents of the specified register pair are saved in two bytes of memory 
    indicated by the stack pointer SP

    Condition bits affected: None
*/
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
    }
    else {
        RegisterPair* rp = extractRegPair(p_state, opcode);
        p_state -> memory[p_state -> sp - 2] = *rp->low;
        p_state -> memory[p_state -> sp - 1] = *rp -> high;
        deleteRegPair(rp);
    }

    p_state -> sp -= 2;

    return CYCLES(11);
}


/*
    Pop Data Off Stack

    The contents of the specified register pair are restored from two bytes of memory indicated by the
    stack pointer SP. The byte of data at the memory address indicated by the stack pointer is loaded into the second
    register of the register pair; the byte of data at the address one greater than the address indicated by the stack pointer
    is loaded into the first register of the pair. If register pair PSW is specified, the byte of data indicated by the contents
    of the stack pointer plus one is used to restore the values of the five condition bits 
    (Carry, Zero, Sign, Parity, and Auxiliary Carry) using the format described in the last section

    Condition bits affected: If register pair PSW is specified, Carry, Sign, Zero, Parity, and Auxiliary Carry may be
                             changed. Otherwise, none are affected.
*/
int op_POP(CPUState* p_state, byte opcode){
    if((opcode & 0x30) >> 4 == 0x03){
        // handle PSW 
        byte flags = p_state -> memory[p_state -> sp];
        p_state -> reg_a = p_state -> memory[p_state -> sp + 1];

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
        deleteRegPair(rp);
    }

    p_state -> sp += 2;
    return CYCLES(10);
}


/*
    Exchange Stack 

    The contents of the L register are exchanged with the contents of the memory byte whose address is held in the stack pointer SP. 
    The contents of the H register are exchanged with the contents of the memory byte whose address is one greater than that held in the stack pointer.

    Condition bits affected: None
*/
int op_XTHL(CPUState* p_state){
    byte tmp_low = p_state -> reg_l;
    byte tmp_high = p_state -> reg_h;

    p_state -> reg_l = p_state -> memory[p_state -> sp];
    p_state -> reg_h = p_state -> memory[p_state -> sp + 1];

    p_state -> memory[p_state -> sp] = tmp_low;
    p_state -> memory[p_state -> sp + 1] = tmp_high;

    return CYCLES(18);


}


/*
    Load SP from H and L

    The 16 bits of data held in the Hand L registers replace the contents of the stack pointer SP. 
    The contents of the Hand L registers are unchanged

    Conditon bits affected: None
*/
int op_SPHL(CPUState *p_state){
    p_state -> memory[p_state -> sp] = p_state -> reg_l;
    p_state -> memory[p_state -> sp + 1] = p_state -> reg_h;

    return CYCLES(5);
}






/* ****************************  IMMEDIATE INSTRUCTIONS ******************************* */

/*
    Move Immediate Data

    The byte of immediate data is stored in the specified register or memory

    Condition bits affected: None
*/
int op_MVI(CPUState* p_state, byte opcode){
    byte* r = extractReg(p_state, opcode); 
    *r = p_state -> memory[p_state -> pc];
    
    p_state -> pc += 1;
    if(cpu_checkMemOp(opcode)){
        return CYCLES(10);
    } else{
        return CYCLES(7);
    }
}


/*
    Add immeditate to Accumulator

    The byte of immediate data is added to the contents of the Accumulator using two's complement arithmetic

    Condition bits affected: Carry, Sign, Zero, Parity, Aux Carry
*/
int op_ADI(CPUState* p_state, byte opcode){
    byte *immediate = &p_state -> memory[p_state -> pc];
    byte *acc = &p_state -> reg_a;

    if(cpu_isAuxCarry(immediate, acc)){
        p_state -> cc.flag_ac = 1;
    } else{
        p_state -> cc.flag_ac = 0;
    }

    uint16_t res = cpu_add(immediate, acc);
    if((res >> 8) == 1){
        p_state -> cc.flag_cy = 1;
    }
    else{
        p_state -> cc.flag_cy = 0;
    }

    cpu_setFlags(&p_state->cc, acc);

    p_state -> pc += 1;
    return CYCLES(7);
}


/*
    Compare Immediate with Accumulator 

    The byte of immediate data is compared with the accumulator

    Condition bits affected: Carry, Sign, Zero, Parity, Aux Carry
*/
int op_CPI(CPUState* p_state, byte opcode){
    uint16_t immediate = p_state -> memory[p_state -> pc];
    byte *acc = &p_state -> reg_a;
    byte twos_comp  = ~immediate + 1;

    uint16_t cmp = (uint16_t) *acc + (uint16_t) twos_comp;

    if(cpu_isAuxCarry(&twos_comp, acc)){
        p_state -> cc.flag_ac = 1;
    } else {
        p_state -> cc.flag_ac = 0;
    }

    if((immediate ^ p_state -> reg_a) & 0x80){
        // different sign
        if((cmp & 0x0100) == 0x0100){
            // overflow
            p_state -> cc.flag_cy = 1;
        } else{
            // no overflow
            p_state -> cc.flag_cy = 0;
        };
    } else{
        // same sign
        if((cmp & 0x0100) == 0x0100){
            // overflow -> no borrow
            p_state -> cc.flag_cy = 0;
        } else{
            // no overflow -> borrow
            p_state -> cc.flag_cy = 1;
        };
    }

    byte res = (byte) cmp;
    cpu_setFlags(&p_state -> cc, &res);

    p_state -> pc += 1;
    return CYCLES(7);
}


/*
    AND Immediate With Accumulator

    A bitwise AND operation is perfromed between the Accumulator and 1 byte of Immediate data

    Condition bits affected: Carry, Zero, Sign, Parity
*/
int op_ANI(CPUState* p_state, byte opcode){

    p_state -> reg_a &= p_state -> memory[p_state -> pc];

    p_state -> cc.flag_cy = 0;

    cpu_setFlags(&p_state->cc, &p_state->reg_a);

    p_state -> pc += 1;

    return CYCLES(7);
}


/*
    Load Immediate Data

    Loads 2 bytes of Immediate data into the register pair specefied.

    Condition bits affected: None
*/
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
    p_state -> pc += 2;

    return CYCLES(10); 
}


/*
    Add Immediate To Accumulator With Carry

    The byte of immediate data is added to the contents of the accumulator plus the contents of the carry bit

    Condition bits affected: Carry, Sign, Zero, 
*/
int op_ACI(CPUState* p_state, byte opcode){
    byte *src = &p_state -> memory[p_state -> pc];
    byte *acc = &p_state -> reg_a;
    byte carry = p_state -> cc.flag_cy;
    byte aux_acc = *acc;

    // capture carry bit from: immediate + accumulator + carry
    uint16_t add_res = cpu_add(src, acc);
    uint16_t res =  add_res +  carry;
    cpu_add(&carry, acc);
    

    // check for carry
    if((res >> 8) == 1){
        p_state -> cc.flag_cy = 1;
    }
    else{
        p_state -> cc.flag_cy = 0;
    }
     

    if(cpu_isAuxCarry(src, &aux_acc)){
        p_state -> cc.flag_ac = 1;
    }
    else{
        byte low_nib = add_res & 0x000f;
        if(cpu_isAuxCarry(&carry, &low_nib)){
            p_state -> cc.flag_ac = 1;
        }
        else{
            p_state -> cc.flag_ac = 0;
        }
    }

    cpu_setFlags(&p_state -> cc, acc);

    p_state -> pc += 1;
    
    return CYCLES(7);
    
}


/*
    Subtract Immediate From Accumulator

    The byte of immediate data is subtracted from the contents of the accumulator using 
    two's complement arithmetic

    Condition bits affected: Carry, Sign, Zero, Parity, Aux Carry
*/
int op_SUI(CPUState* p_state, byte opcode){
    byte *immediate = &p_state -> memory[p_state -> pc]; 
    byte *acc = &p_state -> reg_a;
    byte twos_comp = (~*immediate) + 1;

    if(cpu_isAuxCarry(&twos_comp, acc)){
        p_state -> cc.flag_ac = 1;
    } else{
        p_state -> cc.flag_ac = 0;
    }


    // no carry indicates presence of borrow, thus carry flag represents borrow
    uint16_t res = cpu_add(&twos_comp, acc);
    if((res & 0x0100) == 0x0100){
        p_state -> cc.flag_cy = 0;
    } else{
        p_state -> cc.flag_ac = 1;
    }


    cpu_setFlags(&p_state -> cc, acc);

   p_state -> pc += 1;
    return CYCLES(7);

}


/*
    Subtract Immediate From Accumulator With Borrow

    The Carry bit is internally added to the byte of immediate data. This value is then subtracted from the accumulator using two's complement arithmetic

    Condition bits affected: Carry, Sign, Zero, Parity, Aux Carry
*/
int op_SBI(CPUState* p_state, byte opcode){
    byte *immediate = &p_state -> memory[p_state -> pc]; 
    byte *acc = &p_state -> reg_a;

    byte twos_comp = ~(*immediate + p_state -> cc.flag_cy) + 1;

    if(cpu_isAuxCarry(&twos_comp, acc)){
        p_state -> cc.flag_ac = 1;
    } else{
        p_state -> cc.flag_ac = 0;
    }


    // no carry indicates presence of borrow, thus carry flag represents borrow
    uint16_t res = cpu_add(&twos_comp, acc);
    if((res & 0x0100) == 0x0100){
        p_state -> cc.flag_cy = 0;
    } else{
        p_state -> cc.flag_ac = 1;
    }


    cpu_setFlags(&p_state -> cc, acc);
    p_state -> pc += 1;
    return CYCLES(7);
}


/*
    Exclusive-Or Immediate With Accumulator

    The byte of immediate data is EXCLUSIV E-ORed with the contents of the accumulator. The carry bit is set to zero

    Condition bits affected: Carry, Zero, Sign, Parity
*/
int op_XRI(CPUState* p_state, byte opcode){
    byte *immediate = &p_state->memory[p_state -> pc];
    byte *acc = &p_state -> reg_a;

    *acc ^= *immediate;

    p_state -> cc.flag_cy = 0;
    cpu_setFlags(&p_state -> cc, acc);

    p_state -> pc += 1;
    return CYCLES(7);
}


/*
    Or Immediate With Accumulator

    The byte of immediate data is logically ORed with the contents of the accumulator

    Condition bits affected: Carry, Zero, Sign, Parity
*/
int op_ORI(CPUState* p_state, byte opcode){
    byte *immediate = &p_state->memory[p_state -> pc];
    byte *acc = &p_state -> reg_a;

    *acc |= *immediate;

    p_state -> cc.flag_cy = 0;
    cpu_setFlags(&p_state -> cc, acc);

    p_state -> pc += 1;
    return CYCLES(7);
}













/* ****************************  DIRECT ADDRESSING INSTRUCTIONS ******************************* */

/*
    Store Accumulator Direct

    The contents of the accumulator replace the byte at the memory address formed by concatenating mem[pc  + 1] (High) with mem[pc] (Low)

    Condition bits affected: None
*/
int op_STA(CPUState* p_state, byte opcode){
    byte high = p_state -> memory[p_state -> pc + 1];
    byte low = p_state -> memory[p_state ->  pc];

    uint16_t addr = u8_to_u16(high, low);
    p_state -> memory[addr] = p_state -> reg_a;

    p_state -> pc += 2;
    return CYCLES(13);
}

/*
    Load Accumulator Direct

    The byte at the memory address formed by concatenating mem[pc + 1 ] (HI) with mem[pc] (LOW) replaces the contents of the accumulator.

    Condtion bits affected: None
*/
int op_LDA(CPUState* p_state, byte opcode){
    byte high = p_state -> memory[p_state -> pc + 1];
    byte low = p_state -> memory[p_state ->  pc];

    uint16_t addr = u8_to_u16(high, low);
    p_state -> reg_a = p_state -> memory[addr];

    p_state -> pc += 2;
    return CYCLES(13);
}


/*
    Store H and L Direct

    The contents of the L register are stored at the memory address formed by concatenati ng HI AD 0
    with LOW ADO. The contents of the H register are stored at the next higher memory address

    Condition bits affected: None
*/
int op_SHLD(CPUState* p_state, byte opcode){
    byte hi = p_state -> memory[p_state -> pc + 1];
    byte low = p_state -> memory[p_state -> pc];

    uint16_t addr = u8_to_u16(hi, low);
    p_state -> memory[addr] = p_state -> reg_l;
    p_state -> memory[addr + 1] = p_state -> reg_h;

    p_state -> pc += 2;

    return CYCLES(16);
}



/*
    Load H and L Direct

    The byte at the memory address formed by concatenating HI ADD with LOW ADD replaces the contents of the L register. The byte at the next higher memory
    address replaces the contents of the H register

    Condition bits affected: None
*/
int op_LHLD(CPUState* p_state, byte opcode){
    byte hi = p_state -> memory[p_state -> pc + 1];
    byte low = p_state -> memory[p_state -> pc];

    byte addr = u8_to_u16(hi, low);
    p_state -> reg_l = p_state -> memory[addr];
    p_state -> reg_h = p_state -> memory[addr + 1];

    p_state -> pc += 2;

    return CYCLES(16);
}















/* ****************************  JUMP INSTRUCTIONS ******************************* */

/*
    Load Program Counter

    The contents of the H register replace the
    most significant 8 bits of the program counter, and the contents of the L register replace the least significant 8 bits of
    the program counter. This causes program execution to continue at the address contained in the Hand L registers.

    Condition bits affected: None
*/
int op_PCHL(CPUState* p_state){

    uint16_t addr = u8_to_u16(p_state -> reg_h, p_state -> reg_l);
    p_state -> pc = addr;


    return CYCLES(5);
}



/*
    Jump If Not Zero

    If the Zero bit is 0 (most recent calculation did not result in 0), program execution continues at the memory address addr where:
    
    addr = mem[pc + 1] : mem[pc]

    Condition bits affected: None
*/
int op_JNZ(CPUState* p_state){
    if(p_state -> cc.flag_z == 0){
        uint16_t addr = u8_to_u16(p_state->memory[p_state->pc + 1], p_state->memory[p_state->pc]);
        p_state -> pc = addr;
    }
    else{
        p_state -> pc += 2;
    }

    return CYCLES(10);
}

/*
    JUMP

    Program execution continues unconditionally at the memory address addr, where:
    
    addr = mem[pc + 1] : mem[pc]

    Condtion bits affected: None
*/
int op_JMP(CPUState* p_state){
    uint16_t addr = u8_to_u16(p_state->memory[p_state->pc + 1], p_state->memory[p_state->pc]);
    p_state -> pc = addr;
    return CYCLES(10);
}


/*
    Jump If Carry

    If the Carry bit is one, program execution conti nues at the memory address adr.

    Condition bits affected: None
*/
int op_JC(CPUState* p_state){

    if(p_state -> cc.flag_cy){
        byte hi = p_state -> memory[p_state -> pc + 1];
        byte low = p_state -> memory[p_state-> pc];
        uint16_t addr = u8_to_u16(hi, low);
        p_state-> pc = addr;
    }
    else{
        p_state -> pc += 2;
    }

    return CYCLES(10);
}



/*
    Jump If No Carry

    If the Carry bit is zero, program execution conti nues at the memory address adr.

    Condition bits affected: None
*/
int op_JNC(CPUState* p_state){

    if(p_state -> cc.flag_cy == 0){
        byte hi = p_state -> memory[p_state -> pc + 1];
        byte low = p_state -> memory[p_state-> pc];
        uint16_t addr = u8_to_u16(hi, low);
        p_state-> pc = addr;
    } else{
        p_state -> pc += 2;
    }

    return CYCLES(10);
}



/*
    Jump If Zero

    If the zero bit is one, program execution continues at the memory address ad

    Condition bits affected: None
*/
int op_JZ(CPUState* p_state){
    if(p_state -> cc.flag_z == 1){
        uint16_t addr = u8_to_u16(p_state->memory[p_state->pc + 1], p_state->memory[p_state->pc]);
        p_state -> pc = addr;
    }
    else{
        p_state -> pc += 2;
    }

    return CYCLES(10);
}


/*
    Jump If Minus

    If the Sign bit is one (indicating a negative result), program execution continues at the memory
    address adr

    Condition bits affected: None
*/
int op_JM(CPUState* p_state){
    if(p_state -> cc.flag_s){
        uint16_t addr = u8_to_u16(p_state->memory[p_state->pc + 1], p_state->memory[p_state->pc]);
        p_state -> pc = addr;
    }
    else{
        p_state -> pc += 2;
    }

    return CYCLES(10);
}


/*
    Jump If Minus

    If the Sign bit is zero (indicating a positive result), program execution continues at the memory
    address adr

    Condition bits affected: None
*/
int op_JP(CPUState* p_state){
    if(p_state -> cc.flag_s == 0){
        uint16_t addr = u8_to_u16(p_state->memory[p_state->pc + 1], p_state->memory[p_state->pc]);
        p_state -> pc = addr;
    }
    else{
        p_state -> pc += 2;
    }

    return CYCLES(10);
}




/*
    Jump If Parity Even

    If the parity bit is one (indicating a result with even parity), program execution continues at the memory address adr.

    Condition bits affected: None
*/
int op_JPE(CPUState* p_state){

    if(p_state -> cc.flag_p){
        uint16_t addr = u8_to_u16(p_state->memory[p_state->pc + 1], p_state->memory[p_state->pc]);
        p_state -> pc = addr;
    }
    else{
        p_state -> pc += 2;
    }

    return CYCLES(10);
}


/*
    Jump if Parity Odd

    If the Parity bit is zero (indicating a result with odd parity), program execution conti nues at the
    memory address adr.

    Condition bits affected: None
*/
int op_JPO(CPUState* p_state){

    if(p_state -> cc.flag_p == 0){
        uint16_t addr = u8_to_u16(p_state->memory[p_state->pc + 1], p_state->memory[p_state->pc]);
        p_state -> pc = addr;
    }
    else{
        p_state -> pc += 2;
    }

    return CYCLES(10);
}








/* ****************************  CALL SUBROUTINE INSTRUCTIONS *************************** */

/*
    CALL

    A call operation is unconditionally performed to subroutine sub.

    Condition bits affected: None
*/
int op_CALL(CPUState* p_state){
    // CALL addr
    uint16_t addr = u8_to_u16(p_state -> memory[p_state -> pc + 1], p_state -> memory[p_state -> pc] );

    // increment PC before pushing to stack
    p_state -> pc += 2;
    
    

    byte low = p_state -> pc & 0x00ff;
    byte high = (p_state -> pc & 0xff00) >> 8;

    // push PC onto stack
    p_state -> memory[p_state -> sp - 1] = high;
    p_state -> memory[p_state -> sp - 2] = low;
    p_state -> sp -= 2;

    p_state -> pc = addr;

    return CYCLES(17);
}


/*
    Call If Carry

    A call operation is unconditionally performed to subroutine sub

    Condition bits affected: None
*/
int op_CC(CPUState* p_state){

    if(p_state -> cc.flag_cy){

        return op_CALL(p_state);

    } else{
        p_state -> pc += 2;
        return CYCLES(11);
    }

}



/*
    Call If No Carry

    If the Carry bit is zero, a call operation is
    performed to subroutine sub
    
    Condition bits affected: None
*/
int op_CNC(CPUState* p_state){

    if(p_state -> cc.flag_cy == 0){

        return op_CALL(p_state);

    } else{
        p_state -> pc += 2;
        return CYCLES(11);
    }
}


/*
    Call If Zero

    If the Zero bit is zero, a call operation is
    performed to subroutine sub.

    Condition bits affected: None
*/
int op_CZ(CPUState* p_state){
    if(p_state -> cc.flag_z == 0){

        return op_CALL(p_state);

    } else{
        p_state -> pc += 2;
        return CYCLES(11);
    }
}

/*
    Call If Not Zero

    If the Zero bit is zero, a call operation is
    performed to subroutine sub.

    Condition bits affected: None
*/
int op_CNZ(CPUState *p_state){
    if(p_state -> cc.flag_z){

        return op_CALL(p_state);

    } else{
        p_state -> pc += 2;
        return CYCLES(11);
    }
}

/*
    Call If Minus

    If the Sign bit is one (indicating a minus result), a call operation is performed to subrouti ne sub.

    Condition bits affected: None
*/
int op_CM(CPUState *p_state){
    if(p_state -> cc.flag_s){

        return op_CALL(p_state);

    } else{
        p_state -> pc += 2;
        return CYCLES(11);
    }
}


/*
    Call If Plus

    If the Sign bit is zero (indicating a positive result), a call operation is performed to subrouti ne sub.

    Condition bits affected: None
*/
int op_CP(CPUState *p_state){
    if(p_state -> cc.flag_s == 0){

        return op_CALL(p_state);

    } else{
        p_state -> pc += 2;
        return CYCLES(11);
    }
}

/*
    Call If Parity Even

    If the Parity bit is one (indicating even parity), a call operation is performed to subroutine sub

    Condition bits affected: None
*/
int op_CPE(CPUState *p_state){

    if(p_state -> cc.flag_p){

        return op_CALL(p_state);

    } else{
        p_state -> pc += 2;
        return CYCLES(11);
    }

}


/*
    Call If Parity Odd

    If the Parity bit is zero (indicating odd parity), a call operation is performed to subroutine sub

    Condition bits affected: None
*/
int op_CPO(CPUState *p_state){

    if(p_state -> cc.flag_p == 0){

        return op_CALL(p_state);

    } else{
        p_state -> pc += 2;
        return CYCLES(11);
    }

}












/* ****************************  RETURN FROM SUBROUTINE INSTRUCTIONS *************************** */

/*
    Return

    A return operation is unconditionally performed. Thus, execution proceeds with the instruction immediately following the last call instruction.

    Condition bits affected: None
*/
int op_RET(CPUState* p_state){
    byte hi = p_state -> memory[p_state -> sp  + 1];
    byte low = p_state -> memory[p_state -> sp];
    //printf("RET \n\tSP: %04x - Hi: %02x - Low: %02x\n", p_state -> sp, hi, low);
    uint16_t addr = u8_to_u16(hi, low);

    p_state -> sp += 2;

    p_state -> pc = addr;

    return CYCLES(10);
}


/*
    Return If Carry

    If the Carry bit is one, a return operation is performed

    Condition bits affected: None
*/
int op_RC(CPUState *p_state){

    if(p_state -> cc.flag_cy){
        return op_RET(p_state) + 1;
    } else{
        return CYCLES(5);
    }
}


/*
    Return If Not Carry

    If the Carry bit is zero, a return operation is performed

    Condition bits affected: None
*/
int op_RNC(CPUState *p_state){

    if(p_state -> cc.flag_cy == 0){
        return op_RET(p_state) + 1;
    } else{
        return CYCLES(5);
    }
}

/*
    Return If Zero

    If the Zero bit is one, a return operation is performed

    Condition bits affected: None
*/
int op_RZ(CPUState *p_state){

    if(p_state -> cc.flag_z){
        return op_RET(p_state) + 1;
    } else{
        return CYCLES(5);
    }
}

/*
    Return If Not Zero

    If the Zero bit is zero, a return operation is performed

    Condition bits affected: None
*/
int op_RNZ(CPUState *p_state){

    if(p_state -> cc.flag_z == 0){
        return op_RET(p_state) + 1;
    } else{
        return CYCLES(5);
    }
}


/*
    Return If Plus

    If the sign bit is zero, a return operation is performed

    Condition bits affected: None
*/
int op_RP(CPUState *p_state){

    if(p_state -> cc.flag_s == 0){
        return op_RET(p_state) + 1;
    } else{
        return CYCLES(5);
    }
}

/*
    Return If Minus

    If the Sign bit is one, a return operation is performed

    Condition bits affected: None
*/
int op_RM(CPUState *p_state){

    if(p_state -> cc.flag_s){
        return op_RET(p_state) + 1;
    } else{
        return CYCLES(5);
    }
}

/*
    Return If Parity Even

    If the Parity bit is one, a return operation is performed

    Condition bits affected: None
*/
int op_RPE(CPUState *p_state){

    if(p_state -> cc.flag_p){
        return op_RET(p_state) + 1;
    } else{
        return CYCLES(5);
    }
}

/*
    Return If Parity Odd

    If the Parity bit is zero, a return operation is performed

    Condition bits affected: None
*/
int op_RPO(CPUState *p_state){

    if(p_state -> cc.flag_p == 0){
        return op_RET(p_state) + 1;
    } else{
        return CYCLES(5);
    }
}




int op_RST(CPUState *p_state, byte opcode){
    uint16_t isr_addr = 0x0038 & opcode;
    byte low = p_state -> pc & 0x00ff;
    byte hi = (p_state -> pc & 0xff00) >> 8;
    //printf("hi: %02x -- low: %02x -- pc: %02x\n", hi , low, p_state -> pc);

    //printf("%02x\n", opcode);
    //printf("ISR ADDR: %04x\n", isr_addr);

    p_state -> memory[p_state -> sp - 1] = hi;
    p_state -> memory[p_state -> sp - 2] = low;
    p_state -> sp -= 2;

    p_state -> pc = isr_addr;


    return CYCLES(11);
}






/* ****************************  INTERRUPT ENABLE/DISABLE INSTRUCTIONS *************************** */

/*
    Wrapper around EI and DI, where toggle is used control flow

    toggle 1 (EI):
    This instruction sets the INTE flip-flop, enabling the CPU to recognize and respond to interrupts

    toggle 0 (DI):
    This instruction resets the INTE flip-flop, causing the CPU to ignore all interrupts

    Condition bits affected: None
*/
int op_setI(CPUState* p_state, byte toggle){
    p_state -> int_enable = toggle;

    return CYCLES(4);
}











/* ****************************  I/O INSTRUCTIONS *************************** */

/*
    Output

    The contents of the accumulator are sent to output device number exp

    exp : mem[PC]

    Condition bits affected: None
*/
int op_OUT(CPUState* p_state, Device* devices){
    byte device_id = p_state -> memory[p_state -> pc];
    devices_write(device_id, p_state -> reg_a, devices);

    p_state -> pc += 1;
    return CYCLES(10);
}

/*
    Input

    An eight-bit data byte is read from input device number exp and replaces the contents of the accumulator.

    exp: mem[PC]

    Condition bits affected: None
*/
int op_IN(CPUState* p_state, Device* devices){
    byte device_id = p_state -> memory[p_state -> pc];
    devices_read(device_id, &p_state -> reg_a, devices);
    p_state -> pc += 1;
    return CYCLES(10);
}




/* ****************************  HALT INSTRUCTIONS *************************** */
void op_HLT(void){
    printf("Halting Emulator\n");
    exit(EXIT_SUCCESS);
}



void op_unimplemented(CPUState* p_state, byte opcode){
    cpu_memoryDump(p_state, STACK);
    fprintf(stderr, "INSTRUCTION %02x has not been implemented\n", opcode);
    exit(EXIT_FAILURE);
}








