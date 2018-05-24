#include "Multiply.h"

void multiply(struct ARM arm) {
  //stuff
  //memory[registers[15]] = instructiunea mea
  int pc = arm.registers[15];
  int command = arm.memory[pc];
  int bitA = (1 << 21) & command;
  if(bitA) {
    //multiply and accumulate
  } else {
    //just multiply
  }
}
