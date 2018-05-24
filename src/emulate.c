#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "Branch.h"
#include "global.h"

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
  while(arm.memory[pc] != 0) {
    int command = arm.memory[pc];
    int b27 = (1 << 27) & command;
    int b26 = (1 << 26) & command;
    int b25 = (1 << 25) & command;
    int b4  = (1 << 4)  & command;

    if(b27 == 1) {
      //Branch
    } else if(b26 == 1) {
      //Single Data Transfer
    } else if(b25 == 0 && b4 == 1) {
      //Multiply
    } else {
      //Data Processing
    }
  }


  return EXIT_SUCCESS;
}
