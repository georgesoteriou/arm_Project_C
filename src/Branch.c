#include <stdint.h>
#include "Branch.h"
#include "global.h"

void branch(){
    uint32_t *pc = &arm.registers[15]; 
    int32_t offset = ((1 << 24) - 1) & executeCommand;
    //Apply offset
    (*pc) += (offset<<2);
    //Fix pipeline by loading next commands
    fetchCommand = arm.memory[(*pc)];
    decodeCommand = fetchCommand;
    fetchCommand = arm.memory[++(*pc)];
}
