#include <stdint.h>
#include "Branch.h"
#include "global.h"

void branch(){
    int32_t *pc = &arm.registers[15]; 
    int32_t command = executeCommand;
    int32_t offset = ((1 << 24) - 1) & command;
    (*pc) += offset;
}
