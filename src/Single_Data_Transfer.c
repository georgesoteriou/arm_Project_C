#include "Single_Data_Transfer.h"
#include "global.h"
#include<stdio.h>
#include<stdint.h>

void singleDataTransfer(){

int32_t instruction = arm.memory[arm.registers[15]];

int32_t offset        = instruction && (1<<12 - 1);
int32_t offset_flag   = instruction && (1<<25);
int32_t indexing_flag = instruction && (1<<24);
int32_t up_bit_flag   = instruction && (1<<23);
int32_t load_flag     = instruction && (1<<20);
//shift right by 12 positions so I can get Rd
instruction>>12; 
int32_t dest_register = instruction && (1<<4 - 1);
//shift right by  4 positions so I can get Rn
int32_t base_register = instruction>>4;
int32_t PC = 15;

if(offset_flag){ 
    //if the immediate offset flag is set
    //get the Rm between bits 0 and 3
    int32_t Rm = arm.registers[offset & (1<<4 - 1)];
    int32_t shift_type;
    int32_t shift_amount;
    //shift the offset right 4 times so I can use the "Shift" part of the offset
    offset = offset>>4;
    if(offset && 1 == 0){
       //if the bit 4 is 0, it means that the offset is shifted by a constant amount
       offset = offset>>1;
       shift_type = offset & (1<<3 - 1);
       //shift right by 2 positions so we get in offset the shift constant
       shift_amount = offset>>2;
    } else {
       //if the bit 4 is 1, it means the shift is specified by a register
        offset = offset>>1;
        shift_type = offset & (1<<3 - 1);
        //shift once more to get rid of the 0 and have only the address to the shift register
        int32_t Rs = arm.register[offset>>3];
        //took the last byte of the Rs register
        shift_amount = Rs & (1<<9 - 1);
    }
    if(shift_amount){
        
       o 

    }
     
}


}



