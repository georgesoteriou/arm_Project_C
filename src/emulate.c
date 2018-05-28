#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "global.h"
#include "execute.h"
#include "decode.h"

void printBinary(int32_t x){
  int32_t mask =1 << 31;
  for(int i=0; i<32;i++) {
    if((x & mask) == 0 ) {
      printf("0");
    }  else {
       printf("1");
    }
    x = x << 1; //x <<=1;
  }
  printf("\n");
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
  decodeCommand = 0;
  fetchCommand = arm.memory[(*pc)];
  //cycle 1
  decodeCommand = fetchCommand;
  fetchCommand = arm.memory[++(*pc)];
  //main fetch decode execute function
  while(execute != -2) {
    execute_fn(execute);
    execute = decode_fn(decodeCommand);
    decodeCommand = fetchCommand;
    fetchCommand = arm.memory[++(*pc)];
  }
  //end of loop output
  printf("Registers:\n");

  for(int i = 0; i < 17; i++) {
    if(i == 15) {
      printf("PC %i (%0x)", arm.registers[i], arm.registers[i]);
    } else if(i == 16) {
      printf("CPSR %i (%0x)", arm.registers[i], arm.registers[i]);
    } else if(i != 13 && i != 14) {
      printf("$%i %i (%0x)", i, arm.registers[i], arm.registers[i]);
    }

    printf("\n");
  }

  printf("Non-zero memory:\n");

  for(int i = 0; i < 16384; i++) {
    if(arm.memory[i] != 0) {
      printf("%0x: %0x\n", i * 4, arm.memory[i]);
    }
  }

  return EXIT_SUCCESS;
}
