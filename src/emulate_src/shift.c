#include <stdint.h>
#include "global.h"
#include "shift.h"

void lsl(uint32_t* shiftee, uint32_t S_flag){
  if(S_flag != 0){
    uint32_t firstBit = selectBit((*shiftee), 31);
    if(firstBit != 0) {
      arm.registers[CPSR] |= cMask;
    } else {
      if(selectBit(arm.registers[CPSR], cBit) != 0) {
        arm.registers[CPSR] -= cMask;
      }
    }
  }
  (*shiftee) <<= 1;
}

void lsr(uint32_t* shiftee, uint32_t S_flag){
  if(S_flag != 0){
    uint32_t lastBit = selectBit((*shiftee), 0);
    if(lastBit != 0) {
      arm.registers[CPSR] |= cMask;
    } else {
      if(selectBit(arm.registers[CPSR], cBit) != 0) {
        arm.registers[CPSR] -= cMask;
      }
    }
  }
   (*shiftee) >>= 1;
}

void asr(uint32_t* shiftee, uint32_t S_flag){
  uint32_t firstBit = selectBit((*shiftee), 31);
  if(S_flag != 0){
    if(firstBit != 0) {
      arm.registers[CPSR] |= cMask;
    } else {
      if(selectBit(arm.registers[CPSR], cBit) != 0) {
        arm.registers[CPSR] -= cMask;
      }
    }
  }
  (*shiftee) = ((*shiftee) >> 1) | firstBit;
}

void ror(uint32_t* shiftee, uint32_t S_flag){
  uint32_t lastBit = selectBit((*shiftee), 0);
  if(S_flag != 0){
    uint32_t firstBit = selectBit((*shiftee), 31);
    if(firstBit == 0) {
      arm.registers[CPSR] |= cMask;
    } else {
      if(selectBit(arm.registers[CPSR], cBit) != 0) {
        arm.registers[CPSR] -= cMask;
      }
    }
  }
  (*shiftee) = (*shiftee >> 1) | (lastBit << 31);
}

typedef void (*shift)(uint32_t*, uint32_t);

//execution function table for
//the 4 types of shift
static shift shift_table[4] = {
  lsl,
  lsr,
  asr,
  ror
};

void apply_shift(uint32_t shift_amount, uint32_t shift_type, uint32_t* shiftee, uint32_t S_flag){
  while(shift_amount){
    shift_table[shift_type](shiftee, S_flag);
    shift_amount--;
  }
}

void calculateShiftedOperand(uint32_t* operandPointer, uint32_t S_flag){
  //get the Rm between bits 0 and 3
  uint32_t operand = *operandPointer;
  uint32_t Rm = arm.registers[selectBits(operand, regAddrLength, 0)];
  uint32_t shift_type;
  uint32_t shift_amount;
  //shift the operand right 4 times so I can use the "Shift" part of the operand
  operand = operand >> 4;
  if(selectBit(operand, 0) == 0){
    //if the bit 0 is 0, it means that the operand is shifted by a constant amount
    //first we get rid of bit 0 
    //the shift type is 2 bits long, so then we select those
    operand >>= 1;
    shift_type = selectBits(operand, 2, 0);
    //shift right by 2 positions so we get in operand the shift constant
    shift_amount = operand >> 2;
  } else {
    //if the bit 0 is 1, it means the shift is specified by a register
    //first get rid of bit 0
    //the shift type is 2 bits long, so then we select those
    operand >>= 1;
    shift_type = selectBits(operand, 2, 0);
    //shift once more to get rid of the 0 and have only the address to the shift register
    uint32_t Rs = arm.registers[operand >> 3];
    //took the last byte of the Rs register
    shift_amount = selectBits(Rs, immediateLength, 0);  
  }

  if(shift_amount){
    apply_shift(shift_amount, shift_type, &Rm, S_flag);
  }
  *operandPointer = Rm;
}

uint32_t calculateImmediateOperand(uint32_t operand){
  uint32_t imm = selectBits(operand, immediateLength, 0);
  //the last 4 bits of the 12 bit operand specify the rotate ammount
  //divided by 2
  //so we need to get rid of the 8 bit immediate by shifting left
  //and the shift right once to multiply by 2
  uint32_t rotate = (operand >> 8) << 1;
  while(rotate){
    ror(&imm, 0);
    rotate--;
  }
  return imm;
}
