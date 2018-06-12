#include "Data_Processing.h"


  void eor (int a, int b, int s, int rd) {
    int32_t result =  a ^ b;
    if(s != 0) {
      flagsZN(result);
    }
    // V bit not affected
    arm.registers[rd] = result;
  }

  void teq (int a, int b, int s, int rd) {
    int32_t result = a ^ b;
    if(s != 0) {
      flagsZN(result);
    }
  }

  void and (int a, int b, int s, int rd) {
    int32_t result = a & b;
    if(s != 0) {
      flagsZN(result);
    }
    arm.registers[rd] = result;
  }

  void tst (int a, int b, int s, int rd) {
    int32_t result = a & b;
    if(s != 0) {
      flagsZN(result);
    }
  }

  void sub (int a, int b, int s, int rd) {
    int32_t result = a - b;
    if(s != 0) {
      if(a < b) {
        //C bit needs to be 0
        //if C bit is already one we need to clear it
        if(selectBit(arm.registers[CPSR], cBit) != 0) {
          arm.registers[CPSR] -= cMask;
        } else {
          arm.registers[CPSR] |= cMask;
        }
      }
      flagsZN(result);
    }
    arm.registers[rd] = result;
  }

  void cmp(int a, int b, int s, int rd) {
    int32_t result = a - b;
    if(s != 0) {
      if(a < b){
        //C bit needs to be 0
        //if C bit is already one we need to clear it
        if(selectBit(arm.registers[CPSR], cBit) != 0) {
          arm.registers[CPSR] -= cMask;
        }
      } else {
        arm.registers[CPSR] |= cMask;
      }
      flagsZN(result);
    }
  }

  void rsb (int a, int b, int s, int rd) {
    int32_t result = b - a;
    if(s != 0) {
      if(b < a) {
        //C bit needs to be 0
        //if C bit is already one we need to clear it
        if(selectBit(arm.registers[CPSR], cBit) != 0) {
          arm.registers[CPSR] -= cMask;
        }
      } else {
        arm.registers[CPSR] |= cMask;
      }
      flagsZN(result);
    }
    arm.registers[rd] = result;
  }

  void add(int a, int b, int s, int rd) {
    int32_t result = a + b;
    if(s != 0) {
      if(((a > 0) && (b > INT_MAX - a)) || ((a < 0) &&(b < INT_MIN - a))) { 
        arm.registers[CPSR] |= cMask;
      } else {
        //if C bit needs to be 0
        //if C bit is already one we need to clear it
        if(selectBit(arm.registers[CPSR], cBit) != 0) {
          arm.registers[CPSR] -= cMask;
        }
      }
      flagsZN(result);
    }
    arm.registers[rd] = result;
  }

  void orr(int a, int b, int s, int rd) {
    int32_t result = a | b;
    if(s != 0) {
      flagsZN(result);
    }
    arm.registers[rd] = result;
  }

  void mov(int a, int b, int s, int rd) {
    arm.registers[rd] = b;
  }

  //opcode is 4 bits, so there could be 16 operations
  typedef void (*operation)(int, int, int, int);
  static operation op_table[16];

  void initOpTable(void){
    op_table[0]  = and;    //opcode 0000 
    op_table[1]  = eor;    //opcode 0001 
    op_table[2]  = sub;    //opcode 0010
    op_table[3]  = rsb;    //opcode 0011
    op_table[4]  = add;    //opcode 0100
    op_table[8]  = tst;    //opcode 1000
    op_table[9]  = teq;    //opcode 1001
    op_table[10] = cmp;    //opcode 1010
    op_table[12] = orr;    //opcode 1100
    op_table[13] = mov;    //opcode 1101
  }

  void dataProcessing(void) {
    
    initOpTable();

    uint32_t instr = arm.executeCommand;
    int bitI = selectBit(instr, iBit);
    int bitS = selectBit(instr, sBit);
    uint32_t operand = selectBits(instr, operandLength, 0);
    int rd = selectBits(instr, regAddrLength, operandLength);
    int opcode = selectBits(instr, opcodeLength, opcodeStart); 
    uint32_t rn = arm.registers[selectBits(instr, regAddrLength, operandLength + regAddrLength)];
    
  if(bitI != 0) {
    // operand2 is a rotated immediate
    operand = calculateImmediateOperand(operand);  
  } else {
    // operand2 is register
    calculateShiftedOperand(&operand, bitS);
  }

  op_table[opcode](rn,operand, bitS, rd);
}
