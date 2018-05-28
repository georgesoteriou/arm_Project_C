#include "Data_Processing.h"
#include "global.h"
#include <stdint.h>
#include <limits.h>

typedef void (*operation)(int, int, int, int);

void eor (int a, int b, int s, int rd) {
  int32_t result =  a ^ b;
 
  
  if(s != 0) {
    flagsZN(result);
  }

  // V bit not affected
  arm.registers[rd] = result;

}

void teq (int a, int b, int s, int rd) {
  int32_t result = a ^ b;
  
  if(s != 0) {
    flagsZN(result);
  }
}


void and (int a, int b, int s, int rd) {
  int32_t result = a & b;

  if(s != 0) {
    flagsZN(result);
  }

  arm.registers[rd] = result;
}

void tst (int a, int b, int s, int rd) {
  int32_t result = a & b;

  if(s != 0) {
    flagsZN(result);
  }

}

void sub (int a, int b, int s, int rd) {
  
  int32_t result = a - b;

  if(s != 0) {
    if(((b < 0) && (a > INT_MAX + b)) || ((b > 0) && (a < INT_MIN + b))) {
     arm.registers[16] |= (1 << 29);
    } else {
     //C bit needs to be 0
     //if C bit is already one we need to clear it
       if((arm.registers[16] & (1 << 29)) != 0) {
         arm.registers[16] -= (1 << 29);
       }
    }

    flagsZN(result);
  }

  arm.registers[rd] = result;
}

void cmp(int a, int b, int s, int rd) {
  int32_t result = a - b;

  if(s != 0) {
    if(((b < 0) && (a > INT_MAX + b)) || ((b > 0) && (a < INT_MIN + b))) {
     arm.registers[16] |= (1 << 29);
    } else {
     //C bit needs to be 0
     //if C bit is already one we need to clear it
       if((arm.registers[16] & (1 << 29)) != 0) {
         arm.registers[16] -= (1 << 29);
       }
    }

    flagsZN(result);
  }
}

void rsb (int a, int b, int s, int rd) {
  int32_t result = b - a;

  if(s != 0) {
    if(((a < 0) && (b > INT_MAX + a)) || ((a > 0) && (b < INT_MIN + a))) {
     arm.registers[16] |= (1 << 29);
    } else {
     //C bit needs to be 0
     //if C bit is already one we need to clear it
       if((arm.registers[16] & (1 << 29)) != 0) {
         arm.registers[16] -= (1 << 29);
       }
    }

    flagsZN(result);
  }

  arm.registers[rd] = result;
}

void add(int a, int b, int s, int rd) {
  int32_t result = a + b;
  
  if(s != 0) {
    if(((a > 0) && (b > INT_MAX - a)) || ((a < 0) &&(b < INT_MIN - a))) { 
      arm.registers[16] |= (1 << 29);
     } else {
       //if C bit needs to be 0
       //if C bit is already one we need to clear it
       if((arm.registers[16] & (1 << 29)) != 0) {
         arm.registers[16] -= (1<<29);
      }
     }

    flagsZN(result);
  }

  arm.registers[rd] = result;
}

void orr(int a, int b, int s, int rd) {
  int32_t result = a | b;

  if(s != 0) {
    flagsZN(result);
  }

  arm.registers[rd] = result;
}

void mov(int a, int b, int s, int rd) {
  arm.registers[rd] = b;
}


void dataProcessing() {

  static operation op_table[15];
  op_table[0] =  and;
  op_table[1] =  eor;
  op_table[2] =  sub;
  op_table[3] = rsb;
  op_table[4] = add;
  op_table[8] = tst;
  op_table[9] = teq;
  op_table[10] = cmp;
  op_table[12] = orr;
  op_table[13] = mov;

  int32_t instr = executeCommand;
  int bitI = (1 << 25) & instr;
  int bitS = (1 << 20) & instr;
  int opcode = (instr >> 21) & ((1 << 4) - 1); 
  int rd = (instr >> 12) & ((1 << 4) - 1);
  int rn = arm.registers[(instr >> 16) & ((1 << 4) - 1)];

  int32_t operand = ((1 << 12) - 1 ) & instr;
  
  if(bitI != 0) {
    // operand2 is a rotated immediate
    operand = calculateImmediateOperand(operand);  
  } else {
    // operand2 is register
    calculateShiftedOperand(&operand, bitS);
  }

  op_table[opcode](rn,operand, bitS, rd);
}
