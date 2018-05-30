#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
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
    int charNum = 0;
    int lineNum = 0;
    while(currchar != ' '){
      label[charNum] = fgetc(code);
      charNum++;
    }
    if(label[charNum - 1] = ':' ){
      //save LABEL with length charNum-1 and line is lineNum
    }
    while(currchar != '/n'){
      fgetc(code);
    }
    lineNum++;
  }
  fclose(code);

  //Second Pass
  code = fopen(argv[1], "r");
  while(!feof(code)) {
    char currchar = fgetc(code);
    char line[511];
    int charNum = 0;
    while(currchar != '/n'){
      line[charNum] = fgetc(code);
      charNum++;
    }
    //convert command "LINE"

  }

  return EXIT_SUCCESS;
}
