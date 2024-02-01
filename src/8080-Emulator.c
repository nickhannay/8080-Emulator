#include "8080-Emulator.h"
#include "operations.h"
#include "shared.h"
#include "registers.h"


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>


int emulate8080(State8080* p_state){
    while(true){
        byte opcode = p_state->memory[p_state->pc];
        
        executeOp(p_state, opcode);
    }
    
    return 0;
}





int executeOp(State8080* p_state, byte opcode){
    p_state -> pc += 1;
    
    switch(opcode){
        case 0x00:
        break;
    case 0x01:
        op_LXI(p_state, opcode);
        p_state -> pc += 2;
        break;
    case 0x02:
        op_STAX(p_state, opcode);
        break;
    case 0x03:
        break;
    case 0x04:
        printf("INR B\n");
        break;
    case 0x05:
        op_DCR(p_state, opcode);
        break;
    case 0x06:
        op_MVI(p_state, opcode);
        p_state -> pc += 1;
        break;
    case 0x07:
        printf("RLC\n");
        break;
    case 0x08:
        // unsupported
        break;
    case 0x09:
        op_DAD(p_state, opcode);
        break;
    case 0x0a:
        printf("LDAX B\n");
        break;
    case 0x0b:
        printf("DCX B\n");
        break;
    case 0x0c:
        printf("INR C\n");
        break;
    case 0x0d:
        op_DCR(p_state, opcode);
        break;
    case 0x0e:
        op_MVI(p_state, opcode);
        p_state -> pc += 1;
        break;
    case 0x0f:
        op_RAC(p_state, opcode);
        break;
    case 0x10:
        // unsupported
        break;
    case 0x11:
        op_LXI(p_state, opcode);
        p_state -> pc += 2;
        break;
    case 0x12:
        op_STAX(p_state, opcode);
        break;
    case 0x13:
        op_INX(p_state, opcode);
        break;
    case 0x14:
        printf("INR D\n");
        break;
    case 0x15:
        op_DCR(p_state, opcode);
        break;
    case 0x16:
        op_MVI(p_state, opcode);
        p_state -> pc += 1;
        break;
    case 0x17:
        printf("RAL");
        break;
    case 0x18:
        // unsupported 
        break;
    case 0x19:
        op_DAD(p_state, opcode);
        break;
    case 0x1a:
        op_LDAX(p_state, opcode);
        break;
    case 0x1b:
        printf("DCX D\n");
        break;
    case 0x1c:
        printf("INR E\n");
        break;
    case 0x1d:
        op_DCR(p_state, opcode);
        break;
    case 0x1e:
        op_MVI(p_state, opcode);
        p_state -> pc += 1;
        break;
    case 0x1f:
        printf("RAR\n");
        break;
    case 0x20:
        printf("RIM\n");
        break;
    case 0x21:
        op_LXI(p_state, opcode);
        p_state -> pc += 2;
        break;
    case 0x22:
        //printf("SHLD %02x%02x\n", codes[index + 2], codes[index + 1]);
        //printf("\n\ncpdes[index + 1] = %02x ---- codes[index+2] = %02x\n\n", codes[index + 1], codes[index + 2]);
        p_state -> pc += 2;
        break;
    case 0x23:
        op_INX(p_state, opcode);
        break;
    case 0x24:
        printf("INR H\n");
        break;
    case 0x25:
        op_DCR(p_state, opcode);
        break;
    case 0x26:
        op_MVI(p_state, opcode);
        p_state -> pc += 1;
        break;
    case 0x27:
        printf("DAA\n");
        break;
    case 0x28:
        // unsupported
        break;
    case 0x29:
        op_DAD(p_state, opcode);
        break;
    case 0x2a:
        //printf("LHLD %02x%02x\n", codes[index + 2], codes[index + 1]);
        p_state -> pc += 2;
        break;
    case 0x2b:
        printf("DCX H\n");
        break;
    case 0x2c:
        printf("INR L\n");
        break;
    case 0x2d:
        op_DCR(p_state, opcode);
        break;
    case 0x2e:
        op_MVI(p_state, opcode);
        p_state -> pc += 1;
        break;
    case 0x2f:
        printf("CMA");
        break;
    case 0x30:
        printf("SIM");
        break;
    case 0x31:
        op_LXI(p_state, opcode);
        p_state -> pc += 2;
        break;
    case 0x32:
        op_STA(p_state, opcode);
        p_state -> pc += 2;
        break;
    case 0x33:
        op_INX(p_state, opcode);
        break;
    case 0x34:
        printf("INR M\n");
        break;
    case 0x35:
        op_DCR(p_state, opcode);
        break;
    case 0x36:
        op_MVI(p_state, opcode);
        p_state -> pc += 1;
        break;
    case 0x37:
        printf("STC\n");
        break;
    case 0x38:
        // unsupported
        break;
    case 0x39:
        op_DAD(p_state, opcode);
        break;
    case 0x3a:
        op_LDA(p_state, opcode);
        p_state -> pc += 2;
        break;
    case 0x3b:
        printf("DCX SP\n");
        break;
    case 0x3c:
        printf("INR A\n");
        break;
    case 0x3d:
        op_DCR(p_state, opcode);
        break;
    case 0x3e:
        op_MVI(p_state, opcode);
        p_state -> pc += 1;
        break;
    case 0x3f:
        printf("CMC\n");
        break;
    case 0x40:
    case 0x41:
    case 0x42:
    case 0x43:
    case 0x44:
    case 0x45:
    case 0x46:
    case 0x47:
    case 0x48:
    case 0x49:
    case 0x4a:
    case 0x4b:
    case 0x4c:
    case 0x4d:
    case 0x4e:
    case 0x4f:
    case 0x50:
    case 0x51:
    case 0x52:
    case 0x53:
    case 0x54:
    case 0x55:
    case 0x56:
    case 0x57:
    case 0x58:
    case 0x59:
    case 0x5a:
    case 0x5b:
    case 0x5c:
    case 0x5d:
    case 0x5e:
    case 0x5f:
    case 0x60:
    case 0x61:
    case 0x62:
    case 0x63:
    case 0x64:
    case 0x65:
    case 0x66:
    case 0x67:
    case 0x68:
    case 0x69:
    case 0x6a:
    case 0x6b:
    case 0x6c:
    case 0x6d:
    case 0x6e:
    case 0x6f:
    case 0x70:
    case 0x71:
    case 0x72:
    case 0x73:
    case 0x74:
    case 0x75:
        op_MOV(p_state, opcode);
        break;
    case 0x76:
        printf("HLT\n");
        break;
    case 0x77:
    case 0x78:
    case 0x79:
    case 0x7a:
    case 0x7b:
    case 0x7c:
    case 0x7d:
    case 0x7e:
    case 0x7f:
        op_MOV(p_state, opcode);
        break;
    case 0x80:
        printf("ADD B\n");
        break;
    case 0x81:
        printf("ADD C\n");
        break;
    case 0x82:
        printf("ADD D\n");
        break;
    case 0x83:
        printf("ADD E\n");
        break;
    case 0x84:
        printf("ADD H\n");
        break;
    case 0x85:
        printf("ADD L\n");
        break;
    case 0x86:
        printf("ADD M\n");
        break;
    case 0x87:
        printf("ADD A\n");
        break;
    case 0x88:
        printf("ADC B\n");
        break;
    case 0x89:
        printf("ADC C\n");
        break;
    case 0x8a:
        printf("ADC D\n");
        break;
    case 0x8b:
        printf("ADC E\n");
        break;
    case 0x8c:
        printf("ADC H\n");
        break;
    case 0x8d:
        printf("ADC L\n");
        break;
    case 0x8e:
        printf("ADC M\n");
        break;
    case 0x8f:
        printf("ADC A\n");
        break;
    case 0x90:
        printf("SUB B\n");
        break;
    case 0x91:
        printf("SUB C\n");
        break;
    case 0x92:
        printf("SUB D\n");
        break;
    case 0x93:
        printf("SUB E\n");
        break;
    case 0x94:
        printf("SUB H\n");
        break;
    case 0x95:
        printf("SUB L\n");
        break;
    case 0x96:
        printf("SUB M\n");
        break;
    case 0x97:
        printf("SUB A\n");
        break;
    case 0x98:
        printf("SBB B\n");
        break;
    case 0x99:
        printf("SBB C\n");
        break;
    case 0x9a:
        printf("SBB D\n");
        break;
    case 0x9b:
        printf("SBB E\n");
        break;
    case 0x9c:
        printf("SBB H\n");
        break;
    case 0x9d:
        printf("SBB L\n");
        break;
    case 0x9e:
        printf("SBB M\n");
        break;
    case 0x9f:
        printf("SBB A\n");
        break;
    case 0xa0:
    case 0xa1:
    case 0xa2:
    case 0xa3:
    case 0xa4:
    case 0xa5:
    case 0xa6:
    case 0xa7:
        op_ANA(p_state, opcode);
        break;
    case 0xa8:
    case 0xa9:
    case 0xaa:
    case 0xab:
    case 0xac:
    case 0xad:
    case 0xae:
    case 0xaf:
        op_XRA(p_state, opcode);
        break;
    case 0xb0:
        printf("ORA B\n");
        break;
    case 0xb1:
        printf("ORA C\n");
        break;
    case 0xb2:
        printf("ORA D\n");
        break;
    case 0xb3:
        printf("ORA E\n");
        break;
    case 0xb4:
        printf("ORA H\n");
        break;
    case 0xb5:
        printf("ORA L\n");
        break;
    case 0xb6:
        printf("ORA M\n");
        break;
    case 0xb7:
        printf("ORA A\n");
        break;
    case 0xb8:
        printf("CMP B\n");
        break;
    case 0xb9:
        printf("CMP C\n");
        break;
    case 0xba:
        printf("CMP D\n");
        break;
    case 0xbb:
        printf("CMP E\n");
        break;
    case 0xbc:
        printf("CMP H\n");
        break;
    case 0xbd:
        printf("CMP L\n");
        break;
    case 0xbe:
        printf("CMP M\n");
        break;
    case 0xbf:
        printf("CMP A\n");
        break;
    case 0xc0:
        printf("RNZ\n");
        break;
    case 0xc1:
        op_POP(p_state, opcode);
        break;
    case 0xc2:
        op_JNZ(p_state, opcode);
        break;
    case 0xc3:
        op_JMP(p_state, opcode);
        break;
    case 0xc4:
        //printf("CNZ %02x%02x\n", codes[index + 2], codes[index + 1]);
        p_state -> pc += 2;
        break;
    case 0xc5:
        op_PUSH(p_state, opcode);
        break;
    case 0xc6:
        op_ADI(p_state, opcode);
        p_state -> pc += 1;
        break;
    case 0xc7:
        printf("RST 0\n");
        break;
    case 0xc8:
        printf("RZ\n");
        break;
    case 0xc9:
        op_RET(p_state, opcode);
        break;
    case 0xca:
        //printf("JZ %02x%02x\n", codes[index + 2], codes[index + 1]);
        p_state -> pc += 2;
        break;
    case 0xcb:
        // not supported
        break;
    case 0xcc:
        //printf("CZ %02x%02x\n", codes[index + 2], codes[index + 1]);
        p_state -> pc += 2;
        break;
    case 0xcd:
        op_CALL(p_state, opcode);
        break;
    case 0xce:
        //printf("ACI %02x\n", codes[index + 1]);
        p_state -> pc += 1;
        break;
    case 0xcf:
        printf("RST 1\n");
        break;
    case 0xd0:
        printf("RNC\n");
        break;
    case 0xd1:
        op_POP(p_state, opcode);
        break;
    case 0xd2:
        //printf("JNC %02x%02x\n", codes[index + 2], codes[index + 1]);
        p_state -> pc += 2;
        break;
    case 0xd3:
        //printf("OUT %02x\n", codes[index + 1]);
        p_state -> pc += 1;
        break;
    case 0xd4:
        //printf("CNC %02x%02x\n", codes[index + 2], codes[index + 1]);
        p_state -> pc += 2;
        break;
    case 0xd5:
        op_PUSH(p_state, opcode);
        break;
    case 0xd6:
        //printf("SUI %02x\n", codes[index + 1]);
        p_state -> pc += 2;
        break;
    case 0xd7:
        printf("RST 2\n");
        break;
    case 0xd8:
        printf("RC\n");
        break;
    case 0xd9:
        // not supported
        break;
    case 0xda:
        //printf("JC %02x%02x\n", codes[index + 2], codes[index + 1]);
        p_state -> pc += 2;
        break;
    case 0xdb:
        //printf("IN %02x\n", codes[index + 1]);
        p_state -> pc += 1;
        break;
    case 0xdc:
        //printf("CC %02x%02x\n", codes[index + 2], codes[index + 1]);
        p_state -> pc += 2;
        break;
    case 0xdd:
        // not supported
        break;
    case 0xde:
        //printf("SBI %02x\n", codes[index + 1]);
        p_state -> pc += 1;
        break;
    case 0xdf:
        printf("RST 3\n");
        break;
    case 0xe0:
        printf("RPO\n");
        break;
    case 0xe1:
        op_POP(p_state, opcode);
        break;
    case 0xe2:
        //printf("JPO %02x%02x\n", codes[index + 2], codes[index + 1]);
        p_state -> pc += 2;
        break;
    case 0xe3:
        printf("XTHL\n");
        break;
    case 0xe4:
        //printf("CPO %02x%02x\n", codes[index + 2], codes[index + 1]);
        p_state -> pc += 2;
        break;
    case 0xe5:
        op_PUSH(p_state, opcode);
        break;
    case 0xe6:
        //printf("ANI %02x\n", codes[index + 1]);
        p_state -> pc += 1;
        break;
    case 0xe7:
        printf("RST 4\n");
        break;
    case 0xe8:
        printf("RPE\n");
        break;
    case 0xe9:
        printf("PCHL\n");
        break;
    case 0xea:
        //printf("JPE %02x%02x\n", codes[index + 2], codes[index + 1]);
        p_state -> pc += 2;
        break;
    case 0xeb:
        printf("XCHG\n");
        break;
    case 0xec:
        //printf("CPE %02x%02x\n", codes[index + 2], codes[index + 1]);
        p_state -> pc += 2;
        break;
    case 0xed:
        // not supported
        break;
    case 0xee:
        //printf("XRI %02x\n", codes[index + 1]);
        p_state -> pc += 1;
        break;
    case 0xef:
        printf("RST 5\n");
        break;
    case 0xf0:
        printf("RP\n");
        break;
    case 0xf1:
        printf("POP PSW\n");
        // need to handle flags for special case 
        break;
    case 0xf2:
        //printf("JP %02x%02x\n", codes[index + 2], codes[index + 1]);
        p_state -> pc += 2;
        break;
    case 0xf3:
        printf("DI\n");
        break;
    case 0xf4:
        //printf("CP %02x%02x\n", codes[index + 2], codes[index + 1]);
        p_state -> pc += 2;
        break;
    case 0xf5:
        printf("PUSH PSW\n");
        break;
    case 0xf6:
        //printf("ORI %02x\n", codes[index + 1]);
        p_state -> pc += 1;
        break;
    case 0xf7:
        printf("RST 6\n");
        break;
    case 0xf8:
        printf("RM\n");
        break;
    case 0xf9:
        printf("SPHL\n");
        break;
    case 0xfa:
        //printf("JM %02x%02x\n", codes[index + 2], codes[index + 1]);
        p_state -> pc += 2;
        break;
    case 0xfb:
        printf("EI\n");
        break;
    case 0xfc:
        //printf("CM %02x%02x\n", codes[index + 2], codes[index + 1]);
        p_state -> pc += 2;
        break;
    case 0xfd:
        // not supported
        break;
    case 0xfe:
        //printf("CPI %02x\n", codes[index + 1]);
        p_state -> pc += 1;
        break;
    case 0xff:
        printf("RST 7\n");
        break;
    }

    return 0;

}


