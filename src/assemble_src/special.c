#include "special.h"
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
  result = ((int32_t) 14) << 28;
  
  //setting opcode
  result += (((int32_t) 13) << 21);

  //Remove Spaces
  while((*str) == ' '){
    str++;
  }
  
  char* r = strtok(str, ",");
  int32_t rd = atoi(r + 1) << 12;
  int32_t rn = atoi(r + 1);
 
  result += rd;
  result += rn;

  int32_t num = 0;
  char* constant = strtok(NULL, "\0");
  constant += 1;
  if(checkConstType(constant) == 0) {
    num = (atoi(constant) & ((1 << 5) - 1)) << 7;
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
