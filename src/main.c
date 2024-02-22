#include "8080-Emulator.h"
#include <stdio.h>
#include <stdlib.h>

const char* INVADERS_ROM = "../roms/invaders";

int main(int argc, char** argv){
    Emulator8080* emu = emulator_init();
    int bytes_read = emulator_load_ROM(emu, INVADERS_ROM);
    
    if (bytes_read == -1){
        printf("Failed to load file into emulator\n");
        exit(EXIT_FAILURE);
    }
    printf("Loaded file successfully!\n");


    // memory dump
    /*for(int i = 0; i < 65536; i++){
        printf("cell num %04x -- %02x\n", i, emu -> cpu -> memory[i]);
    }*/

    
    emulator_start(emu);



    return 0;
}