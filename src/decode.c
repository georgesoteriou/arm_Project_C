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
typedef int (*cond)(uint32_t, uint32_t, uint32_t);
static cond cond_table[15];


//-------------------DECODE FUNCTION------------------
int decode_fn(uint32_t command) {
    if(command == 0) return -2;
    arm.executeCommand = command;
    uint32_t b27 = (1 << 27) & command;
    uint32_t b26 = (1 << 26) & command;
    uint32_t b25 = (1 << 25) & command;
    uint32_t b4  = (1 << 7)  & command;
    uint32_t b7  = (1 << 4)  & command;
    uint32_t n   = ((1 << 31) & arm.registers[16]) >> 31;
    uint32_t z   = ((1 << 30) & arm.registers[16]) >> 30;
    uint32_t v   = ((1 << 28) & arm.registers[16]) >> 28;

    //cond table init
    cond_table[0]  = eq;
    cond_table[1]  = ne;
    cond_table[10] = ge;
    cond_table[11] = lt;
    cond_table[12] = gt;
    cond_table[13] = le;
    cond_table[14] = al;

    uint32_t cond = (((((1 << 4) - 1) << 28) & command) >> 28);
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
