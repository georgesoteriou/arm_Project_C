#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "Branch.h"
#include "global.h"

uint32_t branchCondTable[] ={
  0,                                    //beq
  (1 << 28),                            //bne
  (1 << 31) + (1 << 29),                //bge
  (1 << 31) + (1 << 29) + (1 << 28),    //blt
  (1 << 31) + (1 << 30),                //bgt 
  (1 << 31) + (1 << 30) + (1 << 28),    //ble
  (1 << 31) + (1 << 30) + (1 << 29)     //b                          
};

uint32_t branch(int id, char* str){
  int32_t address = getAddress(str);
  if(address != -1){
    int32_t offset = address - (currAddress + 8);
    int32_t shiftedOffset = ((offset  & ((1 << 26) - 1)) >> 2) & ((1 << 24) - 1);
    uint32_t branchconst = (1 << 27) + (1 << 25);
    uint32_t cond = branchCondTable[id - BRANCH_FUNCTION_OFFSET];
    return cond + branchconst + shiftedOffset;
  }else{
    return -1;
  }
}
