#include "Single_Data_Transfer.h"
#include "global.h"
#include<stdio.h>
#include<stdint.h>

void loadData(int32_t src, int32_t dest){
    arm.registers[dest] = arm.memory[src];
}

void writeData(int32_t src, int32_t dest){
    arm.memory[dest] = arm.registers[src];
}

void transferData(int32_t src, int32_t dest, int32_t load_flag){
    if(load_flag){
        /*if the flag is set to load, then load the data from the source
        (memory) to the destination register*/
        loadData(src, dest);
    } else{
        /* if the flag is set to write, then switch the parameters for write
        - write from Rd to memory, so dest -> src */
    writeData(dest, src);
    }
}

int32_t calculate_address(int32_t addr, int32_t add_flag, int32_t offset){
    if(add_flag){
        return addr + offset;
    } 
    return addr - offset;
}

void singleDataTransfer(){

int32_t instruction = executeCommand;

int32_t offset        = instruction & ((1 << 12) - 1);
int32_t immediate_flag   = instruction & (1<<25);
int32_t preindexing_flag = instruction & (1<<24);
int32_t up_bit_flag   = instruction & (1<<23);
int32_t load_flag     = instruction & (1<<20);
//shift right by 12 positions so I can get the address to Rn
instruction >>= 12; 
int32_t Rd = instruction & ((1 << 4) - 1);
//shift right by  4 positions so I can get the address to Rn
int32_t Rn = (instruction >> 4) & ((1 << 4) - 1);
int32_t *Rn_address = &arm.registers[Rn];
//int32_t *Rd_address = &arm.registers[Rd];
int32_t base_reg_value = arm.registers[Rn];

if(immediate_flag){ 
    calculateShiftedOperand(&offset, 0);    
} else {
    offset = calculateImmediateOperand(offset);
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









