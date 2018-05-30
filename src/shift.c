#include <stdint.h>
#include "global.h"
#include "shift.h"

void lsl(uint32_t* shiftee, uint32_t S_flag){
  if(S_flag != 0){
    uint32_t Cbit = (*shiftee) & (1 << 31);
    if(Cbit == 1) {
      arm.registers[CPSR] |= (1 << 29);
    } else {
      if((arm.registers[CPSR] & (1 << 29)) != 0) {
        arm.registers[CPSR] -= (1 << 29);
      }
    }
  }
  (*shiftee) <<= 1;
}

void lsr(uint32_t* shiftee, uint32_t S_flag){
  if(S_flag != 0){
    uint32_t Cbit = (*shiftee) & 1;
    if(Cbit != 0) {
      arm.registers[CPSR] |= (1 << 29);
    } else {
      if((arm.registers[CPSR] & (1 << 29)) != 0) {
        arm.registers[CPSR] -= (1 << 29);
      }
    }
  }
   (*shiftee) >>= 1;
}

void asr(uint32_t* shiftee, uint32_t S_flag){
  uint32_t first_bit = (*shiftee) & (1 << 31);
  if(S_flag != 0){
    uint32_t Cbit = (*shiftee) & (1 << 31);
    if(Cbit != 0) {
      arm.registers[CPSR] |= (1 << 29);
    } else {
      if((arm.registers[CPSR] & (1 << 29)) != 0) {
        arm.registers[CPSR] -= (1 << 29);
      }
    }
  }
  (*shiftee) = ((*shiftee)>>1) | first_bit;
}

void ror(uint32_t* shiftee, uint32_t S_flag){
  uint32_t last_bit = (*shiftee) & 1;
  if(S_flag != 0){
    uint32_t Cbit = (*shiftee) & (1 << 31);
    if(Cbit != 1) {
      arm.registers[CPSR] |= (1 << 29);
    } else {
      if((arm.registers[CPSR] & (1 << 29)) != 0) {
        arm.registers[CPSR] -= (1 << 29);
      }
    }
  }
  (*shiftee) = (*shiftee>>1) | (last_bit<<31);
}

typedef void (*shift)(uint32_t*, uint32_t);

//execution function table
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
  uint32_t Rm = arm.registers[operand & ((1 << 4) - 1)];
  uint32_t shift_type;
  uint32_t shift_amount;
  //shift the operand right 4 times so I can use the "Shift" part of the operand
  operand = operand>>4;
  if((operand & 1) == 0){
    //if the bit 4 is 0, it means that the operand is shifted by a constant amount
    operand = operand>>1;
    shift_type = operand & ((1 << 2) - 1);
    //shift right by 2 positions so we get in operand the shift constant
    shift_amount = operand>>2;
  } else {
    //if the bit 4 is 1, it means the shift is specified by a register
    operand = operand>>1;
    shift_type = operand & ((1 << 2) - 1);
    //shift once more to get rid of the 0 and have only the address to the shift register
    uint32_t Rs = arm.registers[operand>>3];
    //took the last byte of the Rs register
    shift_amount = Rs & ((1 << 8) - 1);
  }

  if(shift_amount){
    apply_shift(shift_amount, shift_type, &Rm, S_flag);
  }
  *operandPointer = Rm;
}

uint32_t calculateImmediateOperand(uint32_t operand){
  uint32_t imm = operand & ((1 << 8) - 1);
  uint32_t rotate = (operand >> 8) << 1;
  while(rotate){
    ror(&imm, 0);
    rotate--;
  }
  return imm;
}
