#include "Branch.h"

#define BIT_28 (1 << 28)
#define BIT_29 (1 << 29)
#define BIT_30 (1 << 30)
#define BIT_31 (1 << 31)
#define MASK_26 ((1 << 26) - 1)
#define MASK_24 ((1 << 24) - 1)
#define BRANCH_CONSTANT (1 << 27) + (1 << 25)

uint32_t branchCondTable[] ={
  0,                           //beq
  BIT_28,                      //bne
  BIT_31 + BIT_29,             //bge
  BIT_31 + BIT_29 + BIT_28,    //blt
  BIT_31 + BIT_30,             //bgt 
  BIT_31 + BIT_30 + BIT_28,    //ble
  BIT_31 + BIT_30 + BIT_29     //b                          
};

uint32_t branch(int id, char *str){
  int32_t address = getAddress(str);
  if(address != -1){
    //get cond
    uint32_t cond = branchCondTable[id - BRANCH_FUNCTION_OFFSET];
    //get offset
    //8 is beacuse of PC offset due to pipeline
    int32_t offset = address - (currAddress + 8);
    //use offset to get shifted offset
    int32_t shiftedOffset = ((offset & MASK_26) >> 2) & MASK_24;
    
    return cond + BRANCH_CONSTANT + shiftedOffset;
  } else{
    return -1;
  }
}
