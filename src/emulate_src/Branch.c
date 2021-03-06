#include "Branch.h"


void branch(void){
  uint32_t *pc = &arm.registers[PC]; 
  int32_t offset = selectBits(arm.executeCommand, branchOffsetLength, 0);
  //sign extending offset, which is currently bits 0 - 23
  uint32_t mask = 1 << 23;
  offset = (offset ^ mask) - mask;
  //Apply offset
  arm.registers[PC] += (offset);
  //Fix pipeline by loading next commands
  arm.fetchCommand = arm.memory[(*pc)];
  arm.decodeCommand = arm.fetchCommand;
  arm.fetchCommand = arm.memory[++(*pc)];
}
