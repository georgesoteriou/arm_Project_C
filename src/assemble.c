#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "assemble_src/global.h"
#include "assemble_src/Data_Processing.h"
#include "assemble_src/Multiply.h"
#include "assemble_src/Single_Data_Transfer.h"
#include "assemble_src/Branch.h"
#include "assemble_src/special.h"

#define BUFFER_SIZE 511

typedef uint32_t (*Mnemonic)(int, char*);

const char* mnemonics[23] = {
  "add","sub","rsb","and","eor","orr","mov","tst","teq","cmp","mul","mla","ldr","str","beq","bne","bge","blt","bgt","ble","b","lsl","andeq"
};

int mnemonicID(char* mnemonic){
  for(int i = 0; i < 23; i++){
    if (strcmp(mnemonics[i], mnemonic) == 0) {
      return i;  
    }
  }
  exit(-1);
}

Mnemonic mnemonicFunc(int id) {
  if(id >= SPECIAL_FUNCTION_OFFSET){
    return special;
  }else if(id >= BRANCH_FUNCTION_OFFSET){
    return branch;
  }else if(id >= SDT_FUNCTION_OFFSET){
    return singleDataTransfer;
  }else{
    return dataProcessing;
  }
}

void firstPass(const char* readFile){
  char buffer[ BUFFER_SIZE ];
  //read from file
  FILE* input_file = fopen( readFile, "r" );
  char* label;

  if(input_file == NULL){
    printf("Unable to open file %s\n", readFile );
  }else{
    // Read each line into the buffer
    int currAddress = 0;
    while(fgets(buffer, BUFFER_SIZE, input_file) != NULL ){
      label = strtok(buffer, "\n");
      if(label != NULL && label[strlen(label) - 1] == ':'){
        label[strlen(label) - 1] = '\0';
        addLabel(label, currAddress);
      }else{
        currAddress += 4;
      }
    }
    if(ferror(input_file) ){
      perror( "The following error occurred" );
    }
    fclose(input_file);
  }
}

void secondPass(const char* readFile,const char* writeFile){
  char buffer[ BUFFER_SIZE ];
  //read from file
  FILE* input_file = fopen(readFile, "r" );
  FILE* write_file = fopen(writeFile, "wb");
  char* mnemonic;
  char* line;

  if(input_file == NULL){
    printf("Unable to open file %s\n", readFile );
  }else{
    currAddress = 0;
    // Read each line into the buffer
    while(fgets(buffer, BUFFER_SIZE, input_file) != NULL ){
      mnemonic = strtok(buffer, " ");
      line = strtok(NULL, "\n" );
      //If not a label
      if(mnemonic[strlen(mnemonic) - 1] != '\n'){
        int id = mnemonicID(mnemonic);
        uint32_t result = mnemonicFunc(id)(id, line);
        //Write binary to output file
        fwrite(&result,sizeof(result),1,write_file);
        currAddress += 4;
      }
    }
    if(ferror(input_file) ){
      perror( "The following error occurred" );
      exit(-1);
    }
    if(ferror(write_file) ){
      perror( "The following error occurred" );
      exit(-1);
    }
    fclose(input_file);
    fclose(write_file);
  }
}

int main(int argc, char **argv) {
  assert(argc == 3);

  //INIT Symbol table
  initSymbolTable();

  const char *readFile = argv[1];
  const char *writeFile = argv[2];

  firstPass(readFile);

  secondPass(readFile, writeFile);
  
  //CLEAR SymbolTable
  clearSymbolTable();

  return EXIT_SUCCESS;
}
