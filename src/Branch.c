#include <stdint.h>
#include "Branch.h"
#include "global.h"

void branch(){
    uint32_t *pc = &arm.registers[15]; 
    int32_t offset = ((1 << 24) - 1) & executeCommand;
    uint32_t mask = 1 << 23;
    offset = (offset ^ mask) - mask;
    //Apply offset
    arm.registers[15] += (offset);
    //Fix pipeline by loading next commands
    fetchCommand = arm.memory[(*pc)];
    decodeCommand = fetchCommand;
    fetchCommand = arm.memory[++(*pc)];
}
