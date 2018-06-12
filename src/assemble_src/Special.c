#include "Special.h"
#include "global.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

int checkConstType(char* imm) {
  if(imm + 1 != '\0') {
    if((*imm) == '0' && (*(imm + 1)) == 'x') {
      return 1;
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}

uint32_t lsl(char* str){
  int32_t result = 0;
  
  //setting cond
  result = condAlways << condStart;
  
  //setting opcode to 13, mov's opcode
  result += (((int32_t) 13) << opcodeStart);

  //Remove Spaces
  while((*str) == ' '){
    str++;
  }
  
  //Getting rn and setting it as rd
  //and as shifted register
  char* r = strtok(str, ",");
  //add 1 to rd and rn to get rid of 'r'
  //shift to correct position
  int32_t rd = atoi(r + 1) << 12;
  int32_t rn = atoi(r + 1);
 
  result += rd;
  result += rn;

  //getting shitft ammount
  int32_t num = 0;
  char* constant = strtok(NULL, "\0");
  constant += 1;
  if(checkConstType(constant) == 0) {
    num = atoi(constant) << 7;
  } else {
    num = (strtol(constant, NULL, 16) & ((1 << 5) - 1)) << 7;
  }
  
  result += num;

  return result; 
}

uint32_t andeq(char* str){
  return 0;
}

typedef uint32_t (*Special_f)(char* str);

static Special_f special_fTable[] ={
  lsl , andeq
};

uint32_t special(int hash, char* str){
  return special_fTable[hash - SPECIAL_FUNCTION_OFFSET](str);
}
