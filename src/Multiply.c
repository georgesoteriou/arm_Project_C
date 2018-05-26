#include <stdint.h>
#include "Multiply.h"
#include "global.h"

void multiply() {
  //memory[registers[15]] = instructiunea mea
  int32_t command = arm.memory[arm.registers[15]];
  int32_t bitA = (1 << 21) & command;
  int32_t bitS = (1 << 20) & command;
  int32_t *rd = &arm.registers[(command >> 16) & 0b1111];
  int32_t rs = arm.registers[(command >> 8) & ob1111];
  int32_t rm = arm.registers[command & ob1111];
  int32_t result;

  if (bitA) {
    //multiply and accumulate
    int32_t rn = arm.registers[(command >> 12) & ob1111];
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
