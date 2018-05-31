#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "assemble_src/global.h"

int main(int argc, char **argv) {
  
  assert(argc == 3);

  //read from file
  FILE* code;
  code = fopen(argv[1], "r");
  if(code == NULL) {
    fprintf(stderr, "Can't open input file!\n");
    exit(-1);
  }

  //First Pass
  while(!feof(code)) {
    char currchar = fgetc(code);
    char label[511];
    for(int i = 0; i < 511; i++) {
      label[i] = 0;
    }
    int charNum = 0;
    int lineNum = 0;
    if(currchar == ':'){

      //STORE LABEL using label index 0 - (charNum-1) with address (lineNum * 4)
      //IGNORE LINE BELOW (Just use it to revent error until implementation)
      label[0] = label[0];

      fgetc(code);
      charNum = 0;
      continue;
    }
    if(currchar == '\n'){
      charNum = 0;
      lineNum++;
      continue;
    }
    label[charNum] = currchar;
    charNum++;
  }
  fclose(code);

  //Second Pass
  code = fopen(argv[1], "r");

  char line[511];
  for(int i = 0; i < 511; i++) {
    line[i] = 0;
  }
  int charNum = 0;

  while(!feof(code)) {
    char currchar = fgetc(code);

    if(currchar == '\n'){
      if(line[charNum-1] != ':'){
        char *token = strtok(line, " ,");
        while (token != NULL){
          //SPLIT COMMANDS AND DO STUFF
          //*token is the first letter
          printf("%s ", token);
          token = strtok(NULL, " ,");
        }
        printf("\n");
      }
      for(int i = 0; i < 511; i++) {
        line[i] = 0;
      }
      charNum = 0;
    }else{
      line[charNum] = currchar;
      charNum++;
    }
  }

  //OUTPUT

  return EXIT_SUCCESS;
}
