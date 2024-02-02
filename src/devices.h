#ifndef _DEVICES_H
#define _DEVICES_H

#include "data_types.h"


typedef struct Device{
    void (*write)(byte);
    void (*read)();
}Device;


int devices_write(byte id, byte accumulator, Device* devices);

int devices_read(byte id, byte* accumulator, Device* devices);

Device* devices_init();

int devices_cleanup(Device* devices);




#endif