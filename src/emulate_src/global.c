#include "global.h"

//17 resigers
const int registerSize = 17;
//64kB / 32 bit word = 16384 32 bit memory words
const int memorySize = 16384;
//PC is register 15
const int PC = 15;
//CPSR is register 16
const int CPSR = 16;

//General Constants
const int nMask = (1 << 31);
const int zMask = (1 << 30);
const int cMask = (1 << 29);
const int vMask = (1 << 28);

const int nBit = 31;
const int zBit = 30;
const int cBit = 29;
const int vBit = 28;

const int regAddrLength = 4;
const int opcodeLength = 4;
const int condLength = 4;
const int immediateLength = 8;
const int sdtOffsetLength = 12;
const int operandLength = 12;
const int branchOffsetLength = 24;

const int opcodeStart = 21;
const int condStart = 28;

const int iBit = 25;
const int sBit = 20;
const int lBit = 20;
const int aBit = 21;
const int pBit = 24;
const int uBit = 23;

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

uint32_t selectBit(uint32_t data, uint32_t bit) {
  return (1 << bit) & data;
}

uint32_t selectBits(uint32_t data, uint32_t amount, uint32_t offset) {
  return ((((1 << amount) - 1) << offset) & data) >> offset;
}

//Set Z and N flags
void flagsZN(int32_t result) {
  //Z bit
  if(result == 0) {
    arm.registers[CPSR] |= zMask;
  } else {
    if(selectBit(arm.registers[CPSR], zBit) != 0) {
      arm.registers[CPSR] -= zMask;
    }
  }
  
  if(result < 0){
    //set N bit
    uint32_t rnbit = selectBit(result, nBit);
    uint32_t nbit = selectBit(arm.registers[CPSR], nBit);
    if(rnbit == 0 && nbit != 0) {
      arm.registers[CPSR] -= nMask;
    } else { 
      arm.registers[CPSR] |= nMask;
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
    uint32_t mask2 = ~mask1;
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
  return (memAddr >= 0) && (memAddr < memorySize);
}


