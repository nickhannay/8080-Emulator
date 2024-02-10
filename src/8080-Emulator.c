#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "Disassembler.h"
#include "8080-Emulator.h"

#define CYCLES_PER_USEC 2


Emulator8080* emulator_init(){
    Emulator8080* emu = calloc(1, sizeof(Emulator8080));
    emu -> cpu = cpu_init();
    emu -> devices = devices_init();

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
    devices_cleanup(emu -> devices);
}




int emulator_start(Emulator8080* emu){
    struct timeval now;
    long long elapsed_usec, elapsed_cycles, elapsed_interrupt_usec = 0;
    while(true) {
        gettimeofday(&now, NULL);

        
        elapsed_usec = (now.tv_sec - emu -> cpu -> tm -> tv_sec ) * 1000000LL +
                       (now.tv_usec - emu -> cpu -> tm -> tv_usec);

        elapsed_cycles = elapsed_usec * CYCLES_PER_USEC;
        int cycles_executed = 0;
        while (cycles_executed < elapsed_cycles){
            byte opcode = cpu_fetch(emu -> cpu);
            printOpCode(emu -> cpu -> memory, emu -> cpu -> pc - 1);

            cycles_executed += cpu_execute(emu -> cpu, opcode, emu -> devices);

        }

        // check for interrupt
        elapsed_interrupt_usec = (now.tv_sec - emu -> cpu -> last_interrupt -> tv_sec ) * 1000000LL +
                                 (now.tv_usec - emu -> cpu -> last_interrupt -> tv_usec);
        if(elapsed_interrupt_usec >= 16666){
            // trigger interrupt
            emu -> cpu -> last_interrupt -> tv_sec = now.tv_sec;
            emu -> cpu -> last_interrupt -> tv_usec = now.tv_usec;
            printf("interrupt triggered every %lli microseconds\n", elapsed_interrupt_usec);
            
        }
        
    }
    
    return 0;
}
