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
    emu -> display = display_init();

    return emu;
}


int emulator_load_ROM(Emulator8080* emu, const char* ROM_file){
    int fd = open(ROM_file, O_RDONLY);
    int bytes_read = read(fd, emu -> cpu -> memory, MEM_SIZE);

    close(fd);

    return bytes_read;
}


void emulator_cleanup(Emulator8080* emu){
    cpu_cleanup(emu -> cpu);
    devices_cleanup(emu -> devices);
    display_cleanup(emu -> display);
    exit(EXIT_SUCCESS);
}




int emulator_start(Emulator8080* emu){
    struct timeval now;
    long long elapsed_usec = 0;
    long long  elapsed_cycles, elapsed_interrupt_cycles = 0;
    long long total_interrupts_count, total_cycle_count = 0;
    SDL_Event event;

    gettimeofday(emu -> cpu -> tm, NULL);
    while(true) {
        gettimeofday(&now, NULL);
        elapsed_usec = (now.tv_sec - emu -> cpu -> tm -> tv_sec ) * 1000000LL +
                       (now.tv_usec - emu -> cpu -> tm -> tv_usec);

        //printf("elapsed usec %lld\n", elapsed_usec);
        elapsed_cycles = elapsed_usec * CYCLES_PER_USEC;
        int cycles_executed = 0;
        while (elapsed_cycles >= 17  && cycles_executed < elapsed_cycles){
            byte opcode = cpu_fetch(emu -> cpu);
            //printOpCode(emu -> cpu -> memory, emu -> cpu -> pc - 1, total_cycle_num);
            int cycles = cpu_execute(emu -> cpu, opcode, emu -> devices);

            cycles_executed += cycles;
            total_cycle_count += cycles;
            elapsed_interrupt_cycles += cycles;
        }

        // trigger interrupt every 16666 cycles (8333 usec)
        if(elapsed_interrupt_cycles >= 16666){
            elapsed_interrupt_cycles = 0;
            switch(emu -> cpu -> int_type){
                // ISR #1 (0x08)
                case HALF_SCREEN:
                    // both interrupts will have fully executed by now -> Draw Screen
                    display_convertBitmap(emu -> cpu, emu -> display -> converted_bitmap);
                    //display_dumpBitmap(emu -> display);
                    display_renderFrame(emu -> display);

                    cpu_execute(emu -> cpu, 0xcf, emu -> devices);
                    emu -> cpu -> int_type = VBLANK;
                    break;
                // ISR #2 (0x10)
                case VBLANK:
                    cpu_execute(emu -> cpu, 0xd7, emu -> devices);
                    emu -> cpu -> int_type = HALF_SCREEN;
                    break;
            }

            // poll for input every half frame (8.33 ms)
            if(SDL_PollEvent(&event)){
                switch(event.type){
                    case SDL_QUIT:
                        printf("\nCycles Executed: %lld\nNumber of Interrupts Handled: %lld\n", total_cycle_count, total_interrupts_count);
                        emulator_cleanup(emu);
                    default:
                        // need to handle other window events
                }
            }
            total_interrupts_count += 1;
        }

    }

    return 0;
}
