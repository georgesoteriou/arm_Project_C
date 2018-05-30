#include <stdint.h>
#include "Branch.h"
#include "global.h"

void branch(void){
  uint32_t *pc = &arm.registers[PC]; 
  int32_t offset = ((1 << 24) - 1) & arm.executeCommand;
  uint32_t mask = 1 << 23;
  offset = (offset ^ mask) - mask;
  //Apply offset
  arm.registers[PC] += (offset);
  //Fix pipeline by loading next commands
  arm.fetchCommand = arm.memory[(*pc)];
  arm.decodeCommand = arm.fetchCommand;
  arm.fetchCommand = arm.memory[++(*pc)];
}
