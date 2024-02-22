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


void emulator_throw_interrupt(Emulator8080* emu, long long *elapsed_interrupt_cycles, long long *frames){
    // trigger interrupt every 8333 usec (120Hz)
    *elapsed_interrupt_cycles = 0;
    switch(emu -> cpu -> int_type){
        // ISR #1 (0x08)
        case HALF_SCREEN:
            cpu_execute(emu -> cpu, 0xcf, emu -> devices);
            emu -> cpu -> int_type = VBLANK;
            break;
        // ISR #2 (0x10)
        case VBLANK:
            // both interrupts will have fully executed by now -> Draw Screen
            display_convertBitmap(emu -> cpu, emu -> display -> converted_bitmap);
            //display_dumpBitmap(emu -> display);
            display_renderFrame(emu -> display);

            *frames += 1;
            cpu_execute(emu -> cpu, 0xd7, emu -> devices);
            emu -> cpu -> int_type = HALF_SCREEN;
            break;
    }

    return;
}




int emulator_start(Emulator8080* emu){
    struct timeval now;
    long long total_interrupts_count = 0, frames = 0;
    SDL_Event event;

    byte opcode = cpu_fetch(emu -> cpu);
    long long prev_instruction_cycles = cpu_execute(emu -> cpu, opcode, emu -> devices);
    long long elapsed_interrupt_cycles = prev_instruction_cycles;
    long long total_cycle_count = prev_instruction_cycles;
    while(true) {
        gettimeofday(&now, NULL);
        long long elapsed_usec = (now.tv_sec - emu -> cpu -> tm -> tv_sec ) * 1000000LL
                                +(now.tv_usec - emu -> cpu -> tm -> tv_usec);

        long long elapsed_cycles = elapsed_usec * CYCLES_PER_USEC;
        if(elapsed_cycles >= prev_instruction_cycles){
            byte opcode = cpu_fetch(emu -> cpu);
            prev_instruction_cycles = cpu_execute(emu -> cpu, opcode, emu -> devices);
            elapsed_interrupt_cycles += prev_instruction_cycles;
            total_cycle_count += prev_instruction_cycles;

            // check for interrupt 
            if(elapsed_interrupt_cycles >= 16666){
                
                total_interrupts_count += 1;
                emulator_throw_interrupt(emu, &elapsed_interrupt_cycles, &frames);

                // Handle window events
                if(SDL_PollEvent(&event)){
                    switch(event.type){
                        case SDL_QUIT:
                            printf("\nCycles Executed: %lld\nNumber of Interrupts Handled: %lld\nNumber of frames drawn %lld\n", total_cycle_count, total_interrupts_count, frames);
                            emulator_cleanup(emu);
                        default:
                            // need to handle other window events
                    }
                }

            }
        }

    }

    return 0;
}
