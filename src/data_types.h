#ifndef _DATA_TYPES_H
#define _DATA_TYPES_H

#include <stdint.h>

typedef uint8_t byte;

#define u8_to_u16(high, low) (((uint16_t) high << 8 ) | low )


#endif