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

int emulator_load(Emulator8080* emu, const char* file);

int emulator_start(Emulator8080* emu);

void emulator_cleanup(Emulator8080* emu);




#endif