#include "shared.h"

// returns 0 on even and 1 on odd
byte getParity(byte input){
    byte ones = 0;
    for(int i = 0; i < 8 ; i++){
        ones += (input >> i ) & 1;
    }

    return ones & 1;
}