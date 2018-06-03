#include "special.h"
#include "global.h"

uint32_t lsl(char* str){
  //TODO: LSL
  return 1; 
}

uint32_t andeq(char* str){
  return 0;
}

typedef uint32_t (*Special_f)(uint32_t);

static Special_f special_fTable[] ={
  lsl , andeq
};

uint32_t special(int hash, char* str){
  return special_fTable[hash - SPECIAL_FUNCTION_OFFSET];
}
