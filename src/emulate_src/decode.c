#include <stdint.h>
#include <stdio.h>
#include "global.h"
#include "decode.h"

//------------COND FUNCTIONS-------------
//Functions to check cond
int eq(uint32_t n, uint32_t z, uint32_t v) {
  return ( (z != 0) ? 1 : 0 );
}
int ne(uint32_t n, uint32_t z, uint32_t v) {
  return ( (z == 0) ? 1 : 0 );
}
int ge(uint32_t n, uint32_t z, uint32_t v ) {
  return ( (n == v) ? 1 : 0 );
}
int lt(uint32_t n, uint32_t z, uint32_t v) {
  return ( (n != v) ? 1 : 0 );
}
int gt(uint32_t n, uint32_t z, uint32_t v) {
  return (((z == 0) && (n == v)) ? 1 : 0 );
}
int le(uint32_t n, uint32_t z, uint32_t v) {
  return (((z != 0) || (n != v)) ? 1 : 0 );
}
int al(uint32_t n, uint32_t z, uint32_t v) {
  return 1;
}

//cond function table declared
//cond field is 4 bits long, so there could be 16
//different conds
typedef int (*cond)(uint32_t, uint32_t, uint32_t);
static cond cond_table[16];

void initCondTable(){
  cond_table[0]  = eq; //cond = 0000 
  cond_table[1]  = ne; //cond = 0001
  cond_table[10] = ge; //cond = 1010
  cond_table[11] = lt; //cond = 1011
  cond_table[12] = gt; //cond = 1100
  cond_table[13] = le; //cond = 1101
  cond_table[14] = al; //cond = 1110
}

//-------------------DECODE FUNCTION------------------
int decode_fn(uint32_t instr) {
  if(instr == 0) {
    return -2;
  }
  
  arm.executeCommand = instr;
  initCondTable();
  
  //in order to differentiate between the 4 types of functions
  //we use the 27th, 26th, 25th, 4th and 7th bits
  uint32_t b27 = selectBit(instr, 27);
  uint32_t b26 = selectBit(instr, 26);
  uint32_t b25 = selectBit(instr, 25);
  uint32_t b4  = selectBit(instr, 7);
  uint32_t b7  = selectBit(instr, 4);
  uint32_t n   = selectBit(arm.registers[CPSR], nBit);
  uint32_t z   = selectBit(arm.registers[CPSR], zBit);
  uint32_t v   = selectBit(arm.registers[CPSR], vBit);

  uint32_t cond = selectBits(instr, condLength, condStart);
  uint32_t flag = cond_table[cond](n,z,v);
  if(flag){
    if(b27 != 0) {
      return 3; 
    } else if(b26 != 0) {
      return 2;
    } else if(b25 == 0 && b7 != 0 && b4 != 0) {
      return 1;
    } else {
      return 0;
    }
  }else{
    return -1;
  }
}
