#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "global.h"
#include "global.c"
#include "Data_Processing.h"
#include "Multiply.h"
#include "Single_Data_Transfer.h"
#include "Branch.h"

//------------EXECUTE FUNCTIONS-------------
//function to execute correct command
typedef int (*execution)(void);

//execution function table
static execution ex_table[4] = {
  dataProcessing,
  multiply,
  singleDataTransfer,
  branch
};

//function to call execution function table
void execute_fn(int execute) {
  if(execute != -1) {
    ex_table[execute]();
  }
} 

//------------COND FUNCTIONS-------------
//Functions to check cond
int eq(int32_t n, int32_t z, int32_t v) {
  return ( (z != 0) ? 1 : 0 );
}
int ne(int32_t n, int32_t z, int32_t v) {
  return ( (z == 0) ? 1 : 0 );
}
int ge(int32_t n, int32_t z, int32_t v ) {
  return ( (n == v) ? 1 : 0 );
}
int lt(int32_t n, int32_t z, int32_t v) {
  return ( (n != v) ? 1 : 0 );
}
int gt(int32_t n, int32_t z, int32_t v) {
  return (((z == 0) && (n == v)) ? 1 : 0 );
}
int le(int32_t n, int32_t z, int32_t v) {
  return (((z != 0) || (n != v)) ? 1 : 0 );
}
int al(int32_t n, int32_t z, int32_t v) {
  return 1;
}

//cond function table declared
typedef int (*cond)(int32_t, int32_t, int32_t);
static cond cond_table[15];


//-------------------DECODE FUNCTION------------------
int decode_fn(int32_t command) {
    if(command == 0) return -2;

    int32_t b27 = (1 << 27) & command;
    int32_t b26 = (1 << 26) & command;
    int32_t b25 = (1 << 25) & command;
    int32_t b4  = (1 << 7)  & command;
    int32_t b7  = (1 << 4)  & command;
    int32_t n   = (1 << 31) & arm.registers[16];
    int32_t z   = (1 << 30) & arm.registers[16];
    int32_t v   = (1 << 28) & arm.registers[16];

    //cond table init
    cond_table[0]  = eq;
    cond_table[1]  = ne;
    cond_table[10] = ge;
    cond_table[11] = lt;
    cond_table[12] = gt;
    cond_table[13] = le;
    cond_table[14] = al;

    int cond = (0b1111 << 28) & command;
    int flag = cond_table[cond](n,z,v);
    if(flag){
      if(b27 != 0) {
        return 3; 
      } else if(b26 != 0) {
        return 2;
      } else if(b25 == 0 && b7 != 0 && b4 != 0) {
        return 1;
      } else {
        return 0;
      }
    }else{
      return -1;
    }
}

int main(int argc, char **argv) {
  //initialise ARM to 0
  for(int i = 0; i < 17; i++){
      arm.registers[i] = 0;
  }
  for(int i = 0; i < 2048; i++){
      arm.memory[i] = 0;
  }

  assert(argc == 2);

  //read from file
  FILE* code;
  code = fopen(argv[1], "r");

  if(code == NULL) {
    fprintf(stderr, "Can't open input file!\n");
    exit(-1);
  }

  //Load program to memmory
  int counter = 0;
  while(!feof(code)) {
    int a = fgetc(code);
    if(a == -1) {
      break;
    }

    arm.memory[counter] = a;
    for(int i = 0; i < 3; i++) {
      arm.memory[counter] <<= 8;
      arm.memory[counter] += fgetc(code);
    }

    counter++;
  }

  //fetch command at 0
  int *pc = &arm.registers[15];
  //cycle 0
  int execute = -1;
  int32_t decode = 0;
  int32_t fetch = arm.memory[(*pc)];
  //cycle 1
  decode = fetch;
  fetch = arm.memory[++(*pc)];
  //main fetch decode execute function
  while(execute != -2) {
    execute_fn(execute);
    execute = decode_fn(decode);
    decode = fetch;
    fetch = arm.memory[++(*pc)];
  }
  //end of loop output
  printf("DONE");

  return EXIT_SUCCESS;
}
