#include <stdint.h>
#include "global.h"

//Convert to and from big/small Endian
uint32_t endianConversion(uint32_t bits) {
  uint32_t mask = (1 << 8) - 1;
  uint32_t result = (bits & mask);
  for(int i = 0; i < 3; i++) {
    result <<= 8;
    bits >>= 8;
    result += (bits & mask);
  }

  return result;
}

//Set Z and N flags
void flagsZN(int32_t result) {
    //Z bit
    if(result == 0) {
        arm.registers[16] |= (1 << 30);
    } else {
        if((arm.registers[16] & (1 << 30)) != 0) {
          arm.registers[16] -= (1 << 30);
        }
    }

    if(result < 0){
        //set N bit
        uint32_t rnbit = (1 << 31) & result;
        uint32_t nbit = (1 << 31) & arm.registers[16];
        if(rnbit == 0 && nbit != 0) {
            arm.registers[16] -= (1 << 31);
        } else { 
            arm.registers[16] |= (1 << 31);
        }
    }
}


//read 4 bytes starting from address and return it
uint32_t getOffsetWord(uint32_t address){
    if((address % 4) == 0) {
      return arm.memory[address / 4];
    } else {
      uint32_t q = address / 4;
      uint32_t r = address % 4;
      uint32_t mem1 = arm.memory[q + 1]; 
      uint32_t mem2 = arm.memory[q];
      uint32_t mask1 = ((1 << (8 * r)) - 1);
      uint32_t mask2 = ((1 << (8 * (4 -r))) - 1) << (8 * r);
      return ((mask2 & mem2) >> (8 * r)) + ((mask1 & mem1) << (8 * (4 - r)));
    }
}

//write 4 byte "data" starting from address
void setOffsetWord(uint32_t address, uint32_t data){
    if((address % 4) == 0) {
      arm.memory[address / 4] = data;
    } else {
      uint32_t q = address / 4;
      uint32_t r = address % 4;
      uint32_t *mem1   = &arm.memory[q + 1]; 
      uint32_t *mem2   = &arm.memory[q];
      uint32_t mask1  = ((1 << (8 * (4-r))) - 1) << (8 * r);
      uint32_t mask2  = (~mask1);
      uint32_t dmask1 = mask1 >> (8 * r);
      uint32_t dmask2 = (~dmask1);

      //clear mem locations
      (*mem1) &= mask1;
      (*mem2) &= mask2;
      
      //set mem
      (*mem2) += ((dmask1 & data) << (8 * r));
      (*mem1) += (dmask2 & data);
    }
}

int32_t isWithinBounds(int32_t memAddr){
    return memAddr >= 0 && memAddr < 16384;
}
