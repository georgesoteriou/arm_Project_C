#include <stdint.h>
#include "decode.h"

//------------COND FUNCTIONS-------------
//Functions to check cond
int eq(int32_t n, int32_t z, int32_t v) {
  return ( (z != 0) ? 1 : 0 );
}
int ne(int32_t n, int32_t z, int32_t v) {
  return ( (z == 0) ? 1 : 0 );
}
int ge(int32_t n, int32_t z, int32_t v ) {
  return ( (n == v) ? 1 : 0 );
}
int lt(int32_t n, int32_t z, int32_t v) {
  return ( (n != v) ? 1 : 0 );
}
int gt(int32_t n, int32_t z, int32_t v) {
  return (((z == 0) && (n == v)) ? 1 : 0 );
}
int le(int32_t n, int32_t z, int32_t v) {
  return (((z != 0) || (n != v)) ? 1 : 0 );
}
int al(int32_t n, int32_t z, int32_t v) {
  return 1;
}

//cond function table declared
typedef int (*cond)(int32_t, int32_t, int32_t);
static cond cond_table[15];


//-------------------DECODE FUNCTION------------------
int decode_fn(int32_t command) {
    if(command == 0) return -2;
    executeCommand = command;
    int32_t b27 = (1 << 27) & command;
    int32_t b26 = (1 << 26) & command;
    int32_t b25 = (1 << 25) & command;
    int32_t b4  = (1 << 7)  & command;
    int32_t b7  = (1 << 4)  & command;
    int32_t n   = (1 << 31) & arm.registers[16];
    int32_t z   = (1 << 30) & arm.registers[16];
    int32_t v   = (1 << 28) & arm.registers[16];

    //cond table init
    cond_table[0]  = eq;
    cond_table[1]  = ne;
    cond_table[10] = ge;
    cond_table[11] = lt;
    cond_table[12] = gt;
    cond_table[13] = le;
    cond_table[14] = al;

    int cond = (0b1111 << 28) & command;
    int flag = cond_table[cond](n,z,v);
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