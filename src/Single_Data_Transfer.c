#include "Single_Data_Transfer.h"
#include "global.h"
#include<stdio.h>
#include<stdint.h>

void loadData(uint32_t src, uint32_t dest){
    arm.registers[dest] = getOffsetWord(src);
}

void writeData(uint32_t src, uint32_t dest){
    arm.memory[dest] = arm.registers[src];
}

void transferData(uint32_t memAddr, uint32_t regAddr, uint32_t load_flag){
    if(isWithinBounds(memAddr)){
    if(load_flag != 0){
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

uint32_t calculate_address(uint32_t addr, uint32_t add_flag, uint32_t offset){
    if(add_flag != 0){
        return addr + offset;
    } 
    return addr - offset;
}

void singleDataTransfer(){

uint32_t instruction = executeCommand;

uint32_t offset        = instruction & ((1 << 12) - 1);
uint32_t immediate_flag   = instruction & (1<<25);
uint32_t preindexing_flag = instruction & (1<<24);
uint32_t up_bit_flag   = instruction & (1<<23);
uint32_t load_flag     = instruction & (1<<20);
//shift right by 12 positions so I can get the address to Rn
instruction >>= 12; 
uint32_t Rd = instruction & ((1 << 4) - 1);
//shift right by  4 positions so I can get the address to Rn
uint32_t Rn = (instruction >> 4) & ((1 << 4) - 1);
uint32_t *Rn_address = &arm.registers[Rn];
//uint32_t *Rd_address = &arm.registers[Rd];
uint32_t base_reg_value = arm.registers[Rn];

if(immediate_flag != 0){ 
    calculateShiftedOperand(&offset, 0);    
} else {
    //offset = calculateImmediateOperand(offset);
}


if(preindexing_flag){
    base_reg_value = calculate_address(base_reg_value, up_bit_flag, offset);
    transferData(base_reg_value, Rd, load_flag);
} else {
    //assert(Rd != Rn);!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    transferData(base_reg_value, Rd, load_flag);
    (*Rn_address) = calculate_address(base_reg_value, up_bit_flag, offset);
}
}









