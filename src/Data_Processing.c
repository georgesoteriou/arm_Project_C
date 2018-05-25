#include "Data_Processing.h"
#include "global.h"
#include<stdint.h>
#include<limits.h>

typedef void (*operation)(int, int, int, int*);

void flagsVZN(int32_t result) {
    //Z bit
    if(result == 0) {
      arm.registres[16] |= (1 << 30);
    } 
  
    //set N bit
    rbit31 = (1 << 31) & result;
    nbit31 = (1 << 31) & arm.registers[16];
    if(bi31 == 0 && nbit31 != 0) {
      arm.registers[16] -= (1 << 31);
    } else {
      arm.registers[16] |= (1 << 31);
    }
}

void eor (int a, int b, int s, int* rd) {
  int32_t result =  a ^ b;
 
  
  if(s != 0) {
    flagsVZN(result);
  }

  // V bit not affected
  *rd = result;

}

void teq (int a, int b, int s, int* rd) {
  int32_t result = a ^ b;
  
  if(s != 0) {
    flagsVZM(result);
  }
}


void and (int a, int b, int s, int* rd) {
  int32_t result = a & b;

  if(s != 0) {
    flagsVZN(result);
  }

  *rd = result;
}

void tst (int a, int b, int s, int* rd) {
  int32_t result = a & b;

  if(s != 0) {
    flagsVZN(result);
  }

}

void sub (int a, int b, int s, int* rd) {
  return a - b;
}

void cmp(int a, int b, int s, int* rd) {

}

void rsb (int a, int b, int s, int* rd) {
  return b - a;
}

void add(int a, int b, int s, int* rd) {
  int32_t result = a + b;
  
  if(s != 0) {
    if(((a > 0) && (b > INT_MAX - a)) || ((a < 0) &&(b < INT_MIN - a))) { 
      arm.registres[16] |= (1 << 29);
     } else {
       //if C bit needs to be 0
       //if C bit is already one we need to clear it
       if((arm.[registers[16]] & (1 << 29)) == 1) {
         arm.registers[16] -= (1<<29);
      }
     }

    flagsVZN(result);
  }

  *rd = result;
}

void or(int a, int b, int s, int* rd) {
  int32_t result = a | b;

  if(s != 0) {
    flagsVZN(result);
  }

  *rd = result;
}

void mov(int a, int b, int s, int* rd) {
  return b;
}


void dataProcessing() {

  static operation op_table[15];
  op_table[0] =  and;
  op_table[1] =  xor;
  op_table[2] =  sub;
  op_table[3] = rsb;
  op_table[4] = add;
  op_table[8] = and;
  op_table[9] = xor;
  op_table[10] = sub;
  op_table[12] = or;
  op_table[13] = mov;

  int32_t instr = executeCommand;
  int bitI = (1 << 25) & instr;
  int bitS = (1 << 20) & instr;
  int opcode = (instr >> 21) & ((1 << 4) - 1); 
  int *rd = &arm.memory[(instr >> 12) & ((1 << 4) - 1)];
  int rn = arm.memory[(instr >> 16) & (1 << 4) - 1];
  int *cpsr = &arm.registers[16];

  int32_t operand = ((1 << 12) - 1 ) & instr;
  
  if(bitI != 0) {
    // operand2 is a rotated immediate
    calculateImmediateOperand(&operand, bitS);  
  } else {
    // operand2 is register
    calculateShiftedOperand(&operand, bitS);
  }

  int32_t result = op_table[opcode](rn,operand, bitS, rd);
}
