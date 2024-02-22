#ifndef _8080_EMULATOR_H
#define _8080_EMULATOR_H

#include "cpu.h"
#include "devices.h"
#include "display.h"


typedef struct Emulator8080{
    CPUState* cpu;
    Device* devices;
    Display* display;
} Emulator8080;




Emulator8080* emulator_init();

int emulator_load_ROM(Emulator8080* emu, const char* file);

void emulator_throw_interrupt(Emulator8080* emu, long long *elapsed_interrupt_cycles, long long *frames);

int emulator_start(Emulator8080* emu);

void emulator_cleanup(Emulator8080* emu);




#endif