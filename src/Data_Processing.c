#include "Data_Processing.h"
#include "global.h"
#include<stdint.h>

typedef int (*operation)(int, int);

int xor (int, a, int b)
{
  return a ^ b;
}

int and (int a, int b) {
  return a & b;
}

int sub (int a, int b) {
  return a - b;
}

int rsb (int a, int b) {
  return b - a;
}

int add(int a, int b) {
  return a + b;
}

int or(int a, int b) {
  return a | b; 
}

int mov(int a, int b) {
  return b;
}

static operation op_table[14] = {
  and,
  xor,
  sub,
  rsb,
  add,
  and,
  xor,
  sub,
  or,
  mov
};

void dataProcessing() {
  int32_t instr = arm.memory[arm.registers[15]];
  int bitI = (1 << 25) & instr;
  int opcode = (instr >> 21) & ((1 << 4) - 1); 
  int rd = arm.memory[(instr >> 12) & ((1 << 4) - 1)];
  
  if(bitI != 0) {
    // operand2 is a constant
    int operand = ((1 << 12) - 1 ) & instr;

    
  } else {
    // operand2 is register
  }
}
