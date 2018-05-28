#include <stdint.h>
#include "global.h"

int32_t endianConversion(int32_t command) {
  int32_t mask = (1 << 8) - 1;
  int32_t result = 0;
  for(int i = 0; i < 4; i++) {
    result += (command & mask);
    result = (result << 8);
    command >>= 8;
  }
  return result;
}

void lsl(int32_t* shiftee, int32_t S_flag){
    if(S_flag != 0){
        int32_t Cbit = (*shiftee) & (1 << 31);
        if(Cbit == 1) {
            arm.registers[16] |= (1 << 29);
        } else {
            if((arm.registers[16] & (1 << 29)) != 0) {
                arm.registers[16] -= (1 << 29);
            }
        }
    }
    (*shiftee) <<= 1;
}
void lsr(int32_t* shiftee, int32_t S_flag){
    if(S_flag != 0){
        int32_t Cbit = (*shiftee) & 1;
        if(Cbit != 0) {
            arm.registers[16] |= (1 << 29);
        } else {
            if((arm.registers[16] & (1 << 29)) != 0) {
                arm.registers[16] -= (1 << 29);
            }
        }
    }
   (*shiftee) >>= 1;
}
void asr(int32_t* shiftee, int32_t S_flag){
    int32_t first_bit = (*shiftee) & (1 << 31);

    if(S_flag != 0){
        int32_t Cbit = (*shiftee) & (1 << 31);
        if(Cbit != 0) {
            arm.registers[16] |= (1 << 29);
        } else {
            if((arm.registers[16] & (1 << 29)) != 0) {
                arm.registers[16] -= (1 << 29);
            }
        }
    }

    (*shiftee) = ((*shiftee)>>1) | first_bit;
}
void ror(int32_t* shiftee, int32_t S_flag){
    int32_t last_bit = (*shiftee) & 1;
   
    if(S_flag != 0){
        int32_t Cbit = (*shiftee) & (1 << 31);
        if(Cbit != 1) {
            arm.registers[16] |= (1 << 29);
        } else {
            if((arm.registers[16] & (1 << 29)) != 0) {
                arm.registers[16] -= (1 << 29);
            }
        }
    }
    
    (*shiftee) = (*shiftee>>1) | (last_bit<<31);
}

void chooseShift(int32_t shiftType, int32_t* shiftee, int32_t S_flag){
    switch(shiftType){
        case 0: lsl(shiftee, S_flag);
                break;
        case 1: lsr(shiftee, S_flag);
                break;
        case 2: asr(shiftee, S_flag);
                break;
        case 3: ror(shiftee, S_flag);
                break;
    }
}

void apply_shift(int32_t shift_amount, int32_t shift_type, int32_t* shiftee, int32_t S_flag){
    while(shift_amount){
        chooseShift(shift_type, shiftee, S_flag);
        shift_amount--;
    }
}

void calculateShiftedOperand(int32_t* operandPointer, int32_t S_flag){
    //get the Rm between bits 0 and 3
    int32_t operand = *operandPointer;
    int32_t Rm = arm.registers[operand & ((1 << 4) - 1)];
    int32_t shift_type;
    int32_t shift_amount;
    //shift the operand right 4 times so I can use the "Shift" part of the operand
    operand = operand>>4;
    if((operand & 1) == 0){
       //if the bit 4 is 0, it means that the operand is shifted by a constant amount
       operand = operand>>1;
       shift_type = operand & ((1 << 3) - 1);
       //shift right by 2 positions so we get in operand the shift constant
       shift_amount = operand>>2;
    } else {
       //if the bit 4 is 1, it means the shift is specified by a register
        operand = operand>>1;
        shift_type = operand & ((1 << 3) - 1);
        //shift once more to get rid of the 0 and have only the address to the shift register
        int32_t Rs = arm.registers[operand>>3];
        //took the last byte of the Rs register
        shift_amount = Rs & ((1 << 9) - 1);
    }

    if(shift_amount){
        apply_shift(shift_amount, shift_type, &Rm, S_flag);
    }

    *operandPointer = Rm;
}

int32_t calculateImmediateOperand(int32_t operand){
    int32_t imm = operand & ((1 << 8) - 1);
    int32_t rotate = (operand >> 8) << 2;
    while(rotate){
        //don't know if to set c flag !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        ror(&imm, 1);
        rotate--;
    }

    return imm;
}


void flagsZN(int32_t result) {
  //Z bit
  if(result == 0) {
    arm.registers[16] |= (1 << 30);
   }
 
  //set N bit
 int32_t rnbit = (1 << 31) & result;
 int32_t nbit = (1 << 31) & arm.registers[16];
 if(rnbit == 0 && nbit != 0) {
    arm.registers[16] -= (1 << 31);
  } else { 
    arm.registers[16] |= (1 << 31);
  }
 }
