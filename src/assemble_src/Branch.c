#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Branch.h"
#include "global.h"

uint32_t beq(uint32_t address){
  uint32_t cond = 0;
  int32_t offset = (address - (currAddress + 8)) & ((1 << 26) - 1);
  return cond | ((offset >> 2) & ((1 << 24) - 1));
}

uint32_t bne(uint32_t address){
  uint32_t cond = (1 << 28);
  int32_t offset = (address - (currAddress + 8)) & ((1 << 26) - 1);
  return cond | ((offset >> 2) & ((1 << 24) - 1));
}

uint32_t bge(uint32_t address){
  uint32_t cond = (1 << 31) + (1 << 29); 
  int32_t offset = (address - (currAddress + 8)) & ((1 << 26) - 1);
  return cond | ((offset >> 2) & ((1 << 24) - 1));
}

uint32_t blt(uint32_t address){
  uint32_t cond = (1 << 31) + (1 << 29) + (1 << 28); 
  int32_t offset = (address - (currAddress + 8)) & ((1 << 26) - 1);
  return cond | ((offset >> 2) & ((1 << 24) - 1));
}

uint32_t bgt(uint32_t address){
  uint32_t cond = (1 << 31) + (1 << 30);
  int32_t offset = (address - (currAddress + 8)) & ((1 << 26) - 1);
  return cond | ((offset >> 2) & ((1 << 24) - 1));
}

uint32_t ble(uint32_t address){
  uint32_t cond = (1 << 31) + (1 << 30) + (1 << 28); 
  int32_t offset = (address - (currAddress + 8)) & ((1 << 26) - 1);
  return cond | ((offset >> 2) & ((1 << 24) - 1));
}

uint32_t b(uint32_t address){
  uint32_t cond = (1 << 31) + (1 << 30) + (1 << 29);
  int32_t offset = (address - (currAddress + 8)) & ((1 << 26) - 1);
  return cond | ((offset >> 2) & ((1 << 24) - 1));
}

typedef uint32_t (*Branch_f)(uint32_t);
static Branch_f branch_fTable[700]; 

void initbranch_fTable(){
  branch_fTable[639] = beq;
  branch_fTable[621] = bne;
  branch_fTable[607] = bge;
  branch_fTable[662] = blt;
  branch_fTable[652] = bgt;
  branch_fTable[617] = ble;
  branch_fTable[137] = b;
}

uint32_t branch(int hash, char* str){
  initbranch_fTable();
  int32_t address = getAddress(str);
  uint32_t result = (1 << 27) + (1 << 25);
  if(address != -1){
    return result + branch_fTable[hash](address);
  }else{
    return -1;
  }
}
