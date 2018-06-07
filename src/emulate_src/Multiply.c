#include <stdint.h>
#include "Multiply.h"
#include "global.h"

void multiply(void) {
  uint32_t instr = arm.executeCommand;
  uint32_t bitA = selectBit(instr, aBit);
  uint32_t bitS = selectBit(instr, sBit);
  uint32_t *rd = &arm.registers[selectBits(instr, regAddrLength, operandLength + regAddrLength)];
  uint32_t rs = arm.registers[selectBits(instr, regAddrLength, immediateLength)];
  uint32_t rm = arm.registers[selectBits(instr, regAddrLength, 0)];
  uint32_t result;

  if (bitA) {
    //multiply and accumulate
    uint32_t rn = arm.registers[selectBits(instr, regAddrLength, operandLength)];
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
