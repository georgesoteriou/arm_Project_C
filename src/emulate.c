#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include "emulate_src/global.h"
#include "emulate_src/execute.h"
#include "emulate_src/decode.h"

int main(int argc, char **argv) {
  initGlobalValues();

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
    uint32_t a = fgetc(code);
    if(a == -1) {
      break;
    }
    arm.memory[counter] = a;
    for(int i = 0; i < 3; i++) {
      arm.memory[counter] <<= 8;
      arm.memory[counter] += fgetc(code);
    }
    if(arm.memory[counter] == 0) {
      eofInst = counter;
    }
    arm.memory[counter] = endianConversion(arm.memory[counter]);
    counter++;
  }

  //fetch command at 0
  uint32_t *pc = &arm.registers[PC];
  //cycle 0
  int execute = -1;
  arm.decodeCommand = 0;
  arm.fetchCommand = arm.memory[(*pc)];
  //cycle 1
  arm.decodeCommand = arm.fetchCommand;
  arm.fetchCommand = arm.memory[++(*pc)];
  //main fetch decode execute function
  while(execute != -2) {
    execute_fn(execute);
    execute = decode_fn(arm.decodeCommand);
    arm.decodeCommand = arm.fetchCommand;
    arm.fetchCommand = arm.memory[++(*pc)];
  }
  //end of loop output
  printf("Registers:\n");

  for(int i = 0; i < 17; i++) {
    if(i == PC) {
      printf("PC  : % 10i (0x%08x)\n", 4 * arm.registers[i], 4 * arm.registers[i]);      
    } else if(i == CPSR) {
      if(((int32_t) arm.registers[i]) < 0) {
        printf("CPSR: % 11i (0x%08x)\n", arm.registers[i], arm.registers[i]);
      } else {
        printf("CPSR:% 11i (0x%08x)\n", arm.registers[i], arm.registers[i]);
      }
    } else if(i != 13 && i != 14) {
      if(((int32_t) arm.registers[i]) < 0) {
        printf("$%-3i: % 10d (0x%08x)\n", i, arm.registers[i], arm.registers[i]);
      } else {
        printf("$%-3i:% 11d (0x%08x)\n", i, arm.registers[i], arm.registers[i]);
      }
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
