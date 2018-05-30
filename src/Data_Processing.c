#include "Data_Processing.h"
#include "global.h"
#include "shift.h"
#include <stdint.h>
#include <limits.h>

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
    //if(((b < 0) && (a > INT_MAX + b)) || ((b > 0) && (a < INT_MIN + b))) {
    if(a < b) {
      //C bit needs to be 0
      //if C bit is already one we need to clear it
      if((arm.registers[CPSR] & (1 << 29)) != 0) {
        arm.registers[CPSR] -= (1 << 29);
      } else {
        arm.registers[CPSR] |= (1 << 29);
      }
    }
    flagsZN(result);
  }
  arm.registers[rd] = result;
}

void cmp(int a, int b, int s, int rd) {
  int32_t result = a - b;
  if(s != 0) {
    //  if(((b < 0) && (a > INT_MAX + b)) || ((b > 0) && (a < INT_MIN + b))) 
    if(a < b){
      //C bit needs to be 0
      //if C bit is already one we need to clear it
      if((arm.registers[CPSR] & (1 << 29)) != 0) {
        arm.registers[CPSR] -= (1 << 29);
      }
    } else {
      arm.registers[CPSR] |= (1 << 29);
    }
    flagsZN(result);
  }
}

void rsb (int a, int b, int s, int rd) {
  int32_t result = b - a;
  if(s != 0) {
    if(b < a) {
      //if(((a < 0) && (b > INT_MAX + a)) || ((a > 0) && (b < INT_MIN + a))) {
      //C bit needs to be 0
      //if C bit is already one we need to clear it
      if((arm.registers[CPSR] & (1 << 29)) != 0) {
        arm.registers[CPSR] -= (1 << 29);
      }
    } else {
      arm.registers[CPSR] |= (1 << 29);
    }
    flagsZN(result);
  }
  arm.registers[rd] = result;
}

void add(int a, int b, int s, int rd) {
  int32_t result = a + b;
  if(s != 0) {
    if(((a > 0) && (b > INT_MAX - a)) || ((a < 0) &&(b < INT_MIN - a))) { 
      arm.registers[CPSR] |= (1 << 29);
    } else {
      //if C bit needs to be 0
      //if C bit is already one we need to clear it
      if((arm.registers[CPSR] & (1 << 29)) != 0) {
        arm.registers[CPSR] -= (1<<29);
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

typedef void (*operation)(int, int, int, int);
static operation op_table[15];

void initOpTable(void){
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
}

void dataProcessing(void) {
  
  initOpTable();

  uint32_t instr = arm.executeCommand;
  int bitI = (1 << 25) & instr;
  int bitS = (1 << 20) & instr;
  int opcode = (instr >> 21) & ((1 << 4) - 1); 
  int rd = (instr >> 12) & ((1 << 4) - 1);
  uint32_t rn = arm.registers[(instr >> 16) & ((1 << 4) - 1)];

  uint32_t operand = ((1 << 12) - 1 ) & instr;
  
  if(bitI != 0) {
  // operand2 is a rotated immediate
  operand = calculateImmediateOperand(operand);  
  } else {
  // operand2 is register
  calculateShiftedOperand(&operand, bitS);
  }

  op_table[opcode](rn,operand, bitS, rd);
}
