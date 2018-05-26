#include <stdint.h>
#include "global.h"

void calculateShiftedOperand(int32_t* operand){





}

void flagsZN(int32_t result) {
     //Z bit
     if(result == 0) {
       arm.registers[16] |= (1 << 30);
     }
 
     //set N bit
     int32_t rnbit = (1 << 31) & result;
     int32_t nbit = (1 << 31) & arm.registers[16];
     if(rnbit == 0 && nbit != 0) {
       arm.registers[16] -= (1 << 31);
     } else {
       arm.registers[16] |= (1 << 31);
     }
 }
