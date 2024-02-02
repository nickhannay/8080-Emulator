#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "Disassembler.h"
#include "8080-Emulator.h"



Emulator8080* emulator_init(){
    Emulator8080* emu = calloc(1, sizeof(Emulator8080));
    emu -> cpu = cpu_init();

    return emu;
}


int emulator_load(Emulator8080* emu, const char* file){
    int fd = open(file, O_RDONLY);
    int bytes_read = read(fd, emu -> cpu -> memory, MEM_SIZE);

    close(fd);

    return bytes_read;
}


void emulator_cleanup(Emulator8080* emu){
    cpu_cleanup(emu -> cpu);
}




int emulator_start(Emulator8080* emu){
    for(int i = 0 ; i < 10000; i++) {
        byte opcode = emu -> cpu -> memory[emu -> cpu ->pc];

        cpu_execute(emu -> cpu, opcode);
    }
    
    return 0;
}







