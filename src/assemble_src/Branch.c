#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Branch.h"
#include "global.h"

uint32_t beq(uint32_t address){
  return 0;
}

uint32_t bne(uint32_t address){
  return (1 << 28);
}

uint32_t bge(uint32_t address){
  return (1 << 31) + (1 << 29); 
}

uint32_t blt(uint32_t address){
  return (1 << 31) + (1 << 29) + (1 << 28); 
}

uint32_t bgt(uint32_t address){
  return (1 << 31) + (1 << 30);
}

uint32_t ble(uint32_t address){
  return (1 << 31) + (1 << 30) + (1 << 28); 
}

uint32_t b(uint32_t address){
  return (1 << 31) + (1 << 30) + (1 << 29);
}

typedef uint32_t (*Branch_f)(uint32_t);

static Branch_f branch_fTable[] ={
  beq,  bne,  bge,  blt,  bgt,  ble,  b
};

uint32_t branch(int id, char* str){
  int32_t address = getAddress(str);
  int32_t offset = address - (currAddress + 8);
  int32_t shiftedOffset = ((offset  & ((1 << 26) - 1)) >> 2) & ((1 << 24) - 1);
  uint32_t branchconst = (1 << 27) + (1 << 25);
  uint32_t cond = branch_fTable[id - BRANCH_FUNCTION_OFFSET](address);
  if(address != -1){
    return cond + branchconst + shiftedOffset;
  }else{
    return -1;
  }
}
