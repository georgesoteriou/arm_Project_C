#include "Data_Processing.h"
#include<stdint.h>

int dataProcessing(struct Arm arm) {
  int32_t instr = arm.memory[arm.registers[15]];
  int bitI = (1 << 25) & instr;
  int opcode = (instr >> 21) & ((1 << 4) - 1); 
  int rd = arm.memory[(instr >> 12) & ((1 << 4) - 1)];
  
  if(bitI != 0) {
    // operand2 is a constant
  } else {
    // operand2 is register
  }
}
