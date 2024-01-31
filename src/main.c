#include <stdio.h>
#include <stdint.h>

#define u8_to_u16(high, low) (((uint16_t) high << 8 ) | low )


int main(){

    printf("%04x\n", u8_to_u16(0xff, 0x93)); 

    return 0;
}


