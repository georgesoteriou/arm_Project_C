#include "Single_Data_Transfer.h"_
#include "global.h"
#include<stdio.h>
#include<stdint.h>

void single_data_transfer(){

int32_t instruction = arm.memory[register[15]];

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

if(offset_flag){ 
    //if the immediate offset flag is set
    //get the Rm between bits 0 and 3
    int32_t Rm = offset && (1<<4 - 1);
    //shift the offset right 4 times so I can use the "Shift" part of the offset
    offset>>4;
    if(offset && 1 == 0){
       //if the bit 4 is 0, it means that the offset is shifted by a constant amount
       offset>>1;
       int32_t shift_type = offset && (1<<3 - 1);
       //shift right by 2 positions so we get in offset the shift constant
       offset>>2;
    } else {
       //if the bit 4 is 1, it means the shift is specified by a register
        offset>>1;
        int32_t shift_type = offset && (1<<3 - 1);
        //shift once more to get rid of the 0 and have only the shift register in offset
        offset>>1;
    }
     
}


}



