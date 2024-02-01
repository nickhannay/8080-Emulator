#include "8080-Emulator.h"
#include "operations.h"
#include "shared.h"
#include "registers.h"
#include "emulator_disassembler.h"


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


State8080* emulator_Init(){
    State8080* emu = calloc(1, sizeof(State8080));
    emu -> memory = calloc(65536 , sizeof(byte));
    emu -> sp = 0x7800;

    return emu;
}


int emulator_Load(State8080* emu, const char* file){
    int fd = open(file, O_RDONLY);
    int bytes_read = read(fd, emu -> memory, 65536);

    close(fd);

    return bytes_read;
}


void emulator_Terminate(State8080* emu){
    free(emu -> memory);
    free(emu);
}




int emulator_Start(State8080* p_state){
    for(int i = 0; i < 25600; i++) {
        byte opcode = p_state->memory[p_state->pc];

        emulator_executeOp(p_state, opcode);
    }
    
    return 0;
}





int emulator_executeOp(State8080* p_state, byte opcode){
    printOpCode(p_state -> memory, p_state -> pc);

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
        break;
    case 0x05:
        op_DCR(p_state, opcode);
        break;
    case 0x06:
        op_MVI(p_state, opcode);
        p_state -> pc += 1;
        break;
    case 0x07:
        break;
    case 0x08:
        // unsupported
        break;
    case 0x09:
        op_DAD(p_state, opcode);
        break;
    case 0x0a:
        op_LDAX(p_state, opcode);
        break;
    case 0x0b:
        break;
    case 0x0c:
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
        break;
    case 0x15:
        op_DCR(p_state, opcode);
        break;
    case 0x16:
        op_MVI(p_state, opcode);
        p_state -> pc += 1;
        break;
    case 0x17:
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
        break;
    case 0x1c:
        break;
    case 0x1d:
        op_DCR(p_state, opcode);
        break;
    case 0x1e:
        op_MVI(p_state, opcode);
        p_state -> pc += 1;
        break;
    case 0x1f:
        break;
    case 0x20:
        break;
    case 0x21:
        op_LXI(p_state, opcode);
        p_state -> pc += 2;
        break;
    case 0x22:
        p_state -> pc += 2;
        break;
    case 0x23:
        op_INX(p_state, opcode);
        break;
    case 0x24:
        break;
    case 0x25:
        op_DCR(p_state, opcode);
        break;
    case 0x26:
        op_MVI(p_state, opcode);
        p_state -> pc += 1;
        break;
    case 0x27:
        break;
    case 0x28:
        // unsupported
        break;
    case 0x29:
        op_DAD(p_state, opcode);
        break;
    case 0x2a:
        p_state -> pc += 2;
        break;
    case 0x2b:
        break;
    case 0x2c:
        break;
    case 0x2d:
        op_DCR(p_state, opcode);
        break;
    case 0x2e:
        op_MVI(p_state, opcode);
        p_state -> pc += 1;
        break;
    case 0x2f:
        break;
    case 0x30:
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
        break;
    case 0x35:
        op_DCR(p_state, opcode);
        break;
    case 0x36:
        op_MVI(p_state, opcode);
        p_state -> pc += 1;
        break;
    case 0x37:
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
        break;
    case 0x3c:
        break;
    case 0x3d:
        op_DCR(p_state, opcode);
        break;
    case 0x3e:
        op_MVI(p_state, opcode);
        p_state -> pc += 1;
        break;
    case 0x3f:
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
        break;
    case 0x81:
        break;
    case 0x82:
        break;
    case 0x83:
        break;
    case 0x84:
        break;
    case 0x85:
        break;
    case 0x86:
        break;
    case 0x87:
        break;
    case 0x88:
        break;
    case 0x89:
        break;
    case 0x8a:
        break;
    case 0x8b:
        break;
    case 0x8c:
        break;
    case 0x8d:
        break;
    case 0x8e:
        break;
    case 0x8f:
        break;
    case 0x90:
        break;
    case 0x91:
        break;
    case 0x92:
        break;
    case 0x93:
        break;
    case 0x94:
        break;
    case 0x95:
        break;
    case 0x96:
        break;
    case 0x97:
        break;
    case 0x98:
        break;
    case 0x99:
        break;
    case 0x9a:
        break;
    case 0x9b:
        break;
    case 0x9c:
        break;
    case 0x9d:
        break;
    case 0x9e:
        break;
    case 0x9f:
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
        break;
    case 0xb1:
        break;
    case 0xb2:
        break;
    case 0xb3:
        break;
    case 0xb4:
        break;
    case 0xb5:
        break;
    case 0xb6:
        break;
    case 0xb7:
        break;
    case 0xb8:
        break;
    case 0xb9:
        break;
    case 0xba:
        break;
    case 0xbb:
        break;
    case 0xbc:
        break;
    case 0xbd:
        break;
    case 0xbe:
        break;
    case 0xbf:
        break;
    case 0xc0:
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
        break;
    case 0xc8:
        break;
    case 0xc9:
        op_RET(p_state, opcode);
        break;
    case 0xca:
        p_state -> pc += 2;
        break;
    case 0xcb:
        // not supported
        break;
    case 0xcc:
        p_state -> pc += 2;
        break;
    case 0xcd:
        op_CALL(p_state, opcode);
        break;
    case 0xce:
        
        p_state -> pc += 1;
        break;
    case 0xcf:
        break;
    case 0xd0:
        break;
    case 0xd1:
        op_POP(p_state, opcode);
        break;
    case 0xd2:
        p_state -> pc += 2;
        break;
    case 0xd3:
        op_OUT(p_state, opcode);
        p_state -> pc += 1;
        break;
    case 0xd4:
        p_state -> pc += 2;
        break;
    case 0xd5:
        op_PUSH(p_state, opcode);
        break;
    case 0xd6:
        
        p_state -> pc += 2;
        break;
    case 0xd7:
        break;
    case 0xd8:
        break;
    case 0xd9:
        // not supported
        break;
    case 0xda:
        p_state -> pc += 2;
        break;
    case 0xdb:
        p_state -> pc += 1;
        break;
    case 0xdc:
        p_state -> pc += 2;
        break;
    case 0xdd:
        // not supported
        break;
    case 0xde:
        
        p_state -> pc += 1;
        break;
    case 0xdf:
        break;
    case 0xe0:
        break;
    case 0xe1:
        op_POP(p_state, opcode);
        break;
    case 0xe2:
        p_state -> pc += 2;
        break;
    case 0xe3:
        break;
    case 0xe4:
        p_state -> pc += 2;
        break;
    case 0xe5:
        op_PUSH(p_state, opcode);
        break;
    case 0xe6:
        op_ANI(p_state, opcode);
        p_state -> pc += 1;
        break;
    case 0xe7:
        break;
    case 0xe8:
        break;
    case 0xe9:
        break;
    case 0xea:
        p_state -> pc += 2;
        break;
    case 0xeb:
        op_XCHG(p_state, opcode);
        break;
    case 0xec:
        p_state -> pc += 2;
        break;
    case 0xed:
        // not supported
        break;
    case 0xee:
        
        p_state -> pc += 1;
        break;
    case 0xef:
        break;
    case 0xf0:
        break;
    case 0xf1:
        op_POP(p_state, opcode);
        break;
    case 0xf2:
        p_state -> pc += 2;
        break;
    case 0xf3:
        // disable interrupts
        op_setI(p_state, 0);
        break;
    case 0xf4:
        p_state -> pc += 2;
        break;
    case 0xf5:
        break;
    case 0xf6:
        
        p_state -> pc += 1;
        break;
    case 0xf7:
        break;
    case 0xf8:
        break;
    case 0xf9:
        break;
    case 0xfa:
        p_state -> pc += 2;
        break;
    case 0xfb:
        // Enable Interrupts
        op_setI(p_state, 1);
        break;
    case 0xfc:
        p_state -> pc += 2;
        break;
    case 0xfd:
        // not supported
        break;
    case 0xfe:
        p_state -> pc += 1;
        break;
    case 0xff:
        break;
    }

    return 0;

}


