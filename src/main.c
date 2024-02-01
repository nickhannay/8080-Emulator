#include "8080-Emulator.h"
#include <stdio.h>
#include <stdlib.h>

const char* INVADERS_ROM = "../roms/invaders";

int main(int argc, char** argv){
    State8080* emu = emulator_Init();
    int bytes_read = emulator_Load(emu, INVADERS_ROM);
    if ( bytes_read == -1){
        printf("Failed to load file into emulator\n");
    }


    // memory dump
    for(int i = 0; i < 65536; i++){
        printf("cell num %d -- %02x\n", i, emu -> memory[i]);
    }

    printf("Loaded file successfully!\n");
    emulator_Start(emu);



    return 0;
}