#include "Single_Data_Transfer.h"
#include "global.h"
#include "shift.h"
#include <stdio.h>
#include <stdint.h>

void loadData(uint32_t src, uint32_t dest){
  arm.registers[dest] = getOffsetWord(src);
}

void writeData(uint32_t src, uint32_t dest){
  setOffsetWord(dest, arm.registers[src]);
}

void transferData(uint32_t memAddr, uint32_t regAddr, uint32_t load_flag){
  if(memAddr == 0x2020008){
    printf("One GPIO pin from 20 to 29 has been accessed\n");
    arm.registers[regAddr] = memAddr;
  }else if(memAddr == 0x20200004){
    printf("One GPIO pin from 10 to 19 has been accessed\n");
    arm.registers[regAddr] = memAddr;
  }else if(memAddr == 0x20200000){
    printf("One GPIO pin from 0 to 9 has been accessed\n");
    arm.registers[regAddr] = memAddr;
  }else{
    if(isWithinBounds(memAddr)) {
      if(load_flag != 0) {
        /*if the flag is set to load, then load the data from the source
        (memory) to the destination register*/
        loadData(memAddr, regAddr);
      } else {
        /* if the flag is set to write, then switch the parameters for write
        - write from Rd to memory, so dest -> src */
        writeData(regAddr, memAddr);
      }
    } else {
      printf("Error: Out of bounds memory access at address 0x%08x\n", memAddr);
    }
  }
}

uint32_t calculate_address(uint32_t addr, uint32_t add_flag, uint32_t offset){
  if(add_flag != 0){
    return addr + offset;
  } 
  return addr - offset;
}

void singleDataTransfer(void){

  uint32_t instr = arm.executeCommand;
  uint32_t offset = selectBits(instr, sdtOffsetLength, 0);
  uint32_t immediate_flag   = selectBit(instr, iBit);
  uint32_t preindexing_flag = selectBit(instr, pBit);
  uint32_t up_bit_flag   = selectBit(instr, uBit);
  uint32_t load_flag     = selectBit(instr, lBit);
  uint32_t Rd = selectBits(instr, regAddrLength, sdtOffsetLength);
  uint32_t Rn = selectBits(instr, regAddrLength, sdtOffsetLength + regAddrLength);
  uint32_t *Rn_address = &arm.registers[Rn];
  uint32_t base_reg_value = arm.registers[Rn];

  if(immediate_flag != 0) { 
    calculateShiftedOperand(&offset, 0);    
  }

  if(Rn == PC) {
    arm.registers[Rd] = arm.memory[eofInst + arm.registers[PC] - 1];
  } else {
    if(preindexing_flag){
      base_reg_value = calculate_address(base_reg_value, up_bit_flag, offset);
      transferData(base_reg_value, Rd, load_flag);
    } else {
      transferData(base_reg_value, Rd, load_flag);
      (*Rn_address) = calculate_address(base_reg_value, up_bit_flag, offset);
    }
  }
}









