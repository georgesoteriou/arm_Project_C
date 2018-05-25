#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "Data_Processing.h"
#include "Multiply.h"
#include "Single_Data_Transfer.h"
#include "Branch.h"
#include "global.h"

typedef int (*execution)(void);

static execution ex_table[4] = {
  dataProcessing,
  multiply,
  singleDataTransfer,
  branch
};

int decode_fn(int command) {
    int b27 = (1 << 27) & command;
    int b26 = (1 << 26) & command;
    int b25 = (1 << 25) & command;
    int b4  = (1 << 4)  & command;

    if(b27 != 0) {
      return 3; 
    } else if(b26 != 0) {
      return 2;
    } else if(b25 == 0 && b4 != 0) {
      return 1;
    } else if(command == 0) {
      return -1;
    } else {
      return 1;
    }
}

//need to deal with cond & with execute being 0/empty
void execute_fn(int decoded) {
  if(decoded != -1) {
    ex_table[decoded]();
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

  //read from file & populate memory
  FILE* code;
  code = fopen(argv[1], "r");

  if(code == NULL) {
    fprintf(stderr, "Can't open input file!\n");
    exit(-1);
  }

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
  
  int pc = arm.registers[15];
  int32_t decode = 0;
  int execute = -1;
  int fetch = arm.memory[pc];
  while(fetch != 0) {
    execute_fn(execute);
    execute = decode_fn(decode);
    decode = fetch;
    fetch = arm.memory[++pc];
  }

  return EXIT_SUCCESS;
}
