#include <stdint.h>
#include "Multiply.h"
#include "global.h"

void multiply() {
  //memory[registers[15]] = instructiunea mea
  uint32_t command = executeCommand;
  int32_t bitA = (1 << 21) & command;
  int32_t bitS = (1 << 20) & command;
  uint32_t *rd = &arm.registers[(command >> 16) & ((1 << 4) - 1)];
  int32_t rs = arm.registers[(command >> 8) & ((1 << 4) - 1)];
  int32_t rm = arm.registers[command & ((1 << 4) - 1)];
  int32_t result;

  if (bitA) {
    //multiply and accumulate
    int32_t rn = arm.registers[(command >> 12) & ((1 << 4) - 1)];
    result = rm * rs + rn;
  } else {
    //just multiply
    result = rm * rs;
  }
  *rd = result;

  if (bitS) {
    flagsZN(result);
  }

}
