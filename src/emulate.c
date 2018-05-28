#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "global.h"
#include "execute.h"
#include "decode.h"

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
    arm.memory[counter] = endianConversion(arm.memory[counter]);
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
      printf("PC  :% 11i (0x%08x)", 4 * arm.registers[i], 4 * arm.registers[i]);
      printf("\n");
      
    } else if(i == 16) {
      printf("CPSR:% 11i (0x%08x)", arm.registers[i], arm.registers[i]);
      printf("\n");
    } else if(i != 13 && i != 14) {
      printf("$%-3i:% 11i (0x%08x)", i, arm.registers[i], arm.registers[i]);
      printf("\n");
    }
  }

  printf("Non-zero memory:\n");

  for(int i = 0; i < 16384; i++) {
    if(arm.memory[i] != 0) {
      printf("0x%08x: 0x%08x\n", i * 4, endianConversion(arm.memory[i]));
    }
  }

  return EXIT_SUCCESS;
}
