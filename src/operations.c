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
    byte *src = extractReg(p_state, opcode);
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
    byte *src = extractReg(p_state, opcode);
    byte *acc = &p_state -> reg_a;
    byte carry = p_state -> cc.flag_cy;
    byte aux_acc = *acc;

    // capture carry bit from: register/memory + accumulator + carry
    uint16_t add_res = cpu_add(src, acc);

    uint16_t res =  add_res +  carry;

    // check for carry
    if((res >> 8) == 1){
        p_state -> cc.flag_cy = 1;
    }
    else{
        p_state -> cc.flag_cy = 0;
    }
     
    // add original carry bit to accumulator 
    // (acc was only set to acc + reg/mem, 1 has not been added yet)
    cpu_add(&carry, acc);




    if(cpu_isAuxCarry(src, &aux_acc)){
        p_state -> cc.flag_ac = 1;
    }
    else{
        byte low_nib = add_res & 0x000f;
        if(cpu_isAuxCarry(carry, &low_nib)){
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







/* ****************************  ROTATE ACCUMULATOR INSTRUCTIONS ******************************* */

int op_RAC(CPUState* p_state, byte opcode){
    byte op = opcode >> 3 & 0x03;
    switch(op){
        case 0x00:
            // RLC
            break;
        case 0x01:
            // RRC
            byte LSB = p_state -> reg_a & 0x01;
            p_state -> cc.flag_cy = LSB;
            p_state -> reg_a >>= 1;
            p_state -> reg_a |= (LSB << 7);
            break;
        case 0x10:
            // RAL
            break;
        case 0x11:
            // RAR
            break;
    }

    return CYCLES(4);
}









/* ****************************  REGISTER PAIR INSTRUCTIONS ******************************* */

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


int op_INX(CPUState* p_state, byte opcode){
    RegisterPair* rp = extractRegPair(p_state, opcode);
    uint16_t combined = u8_to_u16(*rp -> high, *rp -> low);
    combined += 1;
    *rp -> high = (combined & 0xff00 ) >> 8;
    *rp -> low = combined & 0x00ff;


    deleteRegPair(rp);
    return CYCLES(5);
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










/* ****************************  IMMEDIATE INSTRUCTIONS ******************************* */

int op_MVI(CPUState* p_state, byte opcode){
    byte* r = extractReg(p_state, opcode); 
    *r = p_state -> memory[p_state -> pc];
    

    if(cpu_checkMemOp(opcode)){
        return CYCLES(10);
    } else{
        return CYCLES(7);
    }
}


int op_ADI(CPUState* p_state, byte opcode){
    byte *immediate = &p_state -> memory[p_state -> pc];
    byte *acc = &p_state -> reg_a;

    uint16_t res = cpu_add(immediate, acc);
    if((res >> 8) == 1){
        p_state -> cc.flag_cy = 1;
    }
    else{
        p_state -> cc.flag_cy = 0;
    }

    cpu_setFlags(&p_state->cc, acc);


    return CYCLES(7);
}


int op_CPI(CPUState* p_state, byte opcode){
    uint16_t immediate = p_state -> memory[p_state -> pc];

    uint16_t twos_comp  = ~immediate + 1;

    uint16_t cmp = p_state -> reg_a + twos_comp;

    if((immediate ^ p_state -> reg_a) & 0x80){
        // different sign
        if((cmp & 0x10000) == 0x10000){
            // overflow
            p_state -> cc.flag_cy = 1;
        } else{
            // no overflow
            p_state -> cc.flag_cy = 0;
        };
    } else{
        // same sign
        if((cmp & 0x10000) == 0x10000){
            // overflow -> no borrow
            p_state -> cc.flag_cy = 0;
        } else{
            // no overflow -> borrow
            p_state -> cc.flag_cy = 1;
        };
    }

    byte res = (byte) cmp;
    cpu_setFlags(&p_state -> cc, &res);


    return CYCLES(7);
}


/*
    A bitwise AND operation is perfromed between the Accumulator and 1 byte of Immediate data
*/
int op_ANI(CPUState* p_state, byte opcode){

    p_state -> reg_a &= p_state -> memory[p_state -> pc];

    p_state -> cc.flag_cy = 0;

    cpu_setFlags(&p_state->cc, &p_state->reg_a);

    return CYCLES(7);
}

/*
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
    return CYCLES(10); 
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
    return CYCLES(13);
}















/* ****************************  JUMP INSTRUCTIONS ******************************* */

/*
    Jump If Not Zero

    If the Zero bit is 0 (most recent calculation did not result in 0), program execution continues at the memory address addr where:
    
    addr = mem[pc + 1] : mem[pc]

    Condition bits affected: None
*/
int op_JNZ(CPUState* p_state, byte opcode){
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
int op_JMP(CPUState* p_state, byte opcode){
    uint16_t addr = u8_to_u16(p_state->memory[p_state->pc + 1], p_state->memory[p_state->pc]);
    p_state -> pc = addr;
    return CYCLES(10);
}














/* ****************************  CALL SUBROUTINE INSTRUCTIONS *************************** */

/*
    CALL

    A call operation is unconditionally performed to subroutine sub.

    Condition bits affected: None
*/
int op_CALL(CPUState* p_state, byte opcode){
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














/* ****************************  RETURN FROM SUBROUTINE INSTRUCTIONS *************************** */

/*
    Return

    A return operation is unconditionally performed. Thus, execution proceeds with the instruction immediately following the last call instruction.

    Condition bits affected: None
*/
int op_RET(CPUState* p_state, byte opcode){
    uint16_t addr = u8_to_u16(p_state -> memory[p_state -> sp  + 1], p_state -> memory[p_state -> sp]);

    p_state -> sp += 2;

    p_state -> pc = addr;

    return CYCLES(10);
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

    return CYCLES(10);
}









void op_unimplemented(byte opcode){
    fprintf(stderr, "INSTRUCTION %02x has not been implemented\n", opcode);
    exit(EXIT_FAILURE);
}








