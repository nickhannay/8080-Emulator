#include "devices.h"
#include "data_types.h"
#include <stdlib.h>
#include <stdio.h>



/**************************************** Space Invaders Specific ***************************************************/

byte shift_offset = 0;

uint16_t shift_reg = 0;

void write_dev2(byte value){
    shift_offset = value & 0x07;

    printf("writing %02x to PORT: 2 - shift offset = %d\n", value, shift_offset);
    return;
}


void write_dev3(byte value){
    printf("writing %02x to PORT: 3\n", value);
    return;
}


void write_dev4(byte value){
    shift_reg = (uint16_t) value << 8 | (shift_reg >> 8);

    printf("writing %02x to PORT: 4 - shift reg = %04x\n", value, shift_reg);
    return;
}


void write_dev5(byte value){
    printf("writing %02x to PORT: 5\n", value);
    return;
}
void write_dev6(byte value){
    printf("writing %02x to PORT: 6\n", value);
    return;
}


void read_dev1(){
    printf("reading from PORT: 1\n");
    return;
}

void read_dev2(){
    printf("reading from PORT: 2\n");
    return;
}
void read_dev3(){
    byte value = (shift_reg >> ( 8 - shift_offset)) & 0x00ff; 
    printf("%02x\n", value );
    // TO DO


    printf("reading from PORT: 3\n");
    return;
}


/*************************************************************************************************************************************************************/



const int NUM_DEVICES = 256;

Device* devices_init(){
    Device* devices = calloc(NUM_DEVICES, sizeof(Device));

    // set machine specific devices

    // Space Invaders
    devices[2].write =  write_dev2;
    devices[3].write =  write_dev3;
    devices[4].write =  write_dev4;
    devices[5].write =  write_dev5;
    devices[6].write =  write_dev6;

    devices[1].read =  read_dev1;
    devices[2].read =  read_dev2;
    devices[3].read =  read_dev3;


    return devices;
}

int devices_cleanup(Device* devices){
    free(devices);
    return 0;
}

int devices_write(byte id, byte accumulator, Device* devices){
    devices[id].write(accumulator);
    return 0;
}

int devices_read(byte id, byte* accumulator, Device* devices){
    // TO DO
    devices[id].read();

    return 0;
}