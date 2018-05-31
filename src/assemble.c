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
static Mnemonic mnemonicTable[1024]; 

void initMnemonicTable(){
  mnemonicTable[597] = dataProcessing;
  mnemonicTable[643] = dataProcessing;
  mnemonicTable[638] = dataProcessing;
  mnemonicTable[617] = dataProcessing;
  mnemonicTable[665] = dataProcessing;
  mnemonicTable[681] = dataProcessing;
  mnemonicTable[685] = dataProcessing;
  mnemonicTable[694] = dataProcessing;
  mnemonicTable[657] = dataProcessing;
  mnemonicTable[653] = dataProcessing;
  mnemonicTable[667] = multiply;
  mnemonicTable[616] = multiply;
  mnemonicTable[650] = singleDataTransfer;
  mnemonicTable[689] = singleDataTransfer;
  mnemonicTable[639] = branch;
  mnemonicTable[621] = branch;
  mnemonicTable[607] = branch;
  mnemonicTable[662] = branch;
  mnemonicTable[652] = branch;
  mnemonicTable[617] = branch;
  mnemonicTable[137] = branch;
  mnemonicTable[662] = special;
  mnemonicTable[1021]= special;
}

int main(int argc, char **argv) {
  initMnemonicTable();

  //assert(argc == 3);
  const char *filename = argv[1];
  char buffer[ BUFFER_SIZE ];

  //read from file
  FILE* input_file = fopen( filename, "r" );
  char* label;

  initSymbolTable();

  //FIRST PASS
  if(input_file == NULL){
    printf("Unable to open file %s\n", filename );
  }else{
    // Read each line into the buffer
    int adr = 0;
    while(fgets(buffer, BUFFER_SIZE, input_file) != NULL ){
      label = strtok(buffer, "\n");
      if(label[strlen(label) - 2] == ':'){
        label[strlen(label) - 2] = '\0';
        printf("%s\n",label);
        char* labelcopy = NULL;
        strcpy(labelcopy, label);
        addSymbolTable(labelcopy, adr);
      }else{
        adr += 4;
      }
    }
    if(ferror(input_file) ){
      perror( "The following error occurred" );
    }
    fclose(input_file);
  }

  //read from file
  input_file = fopen( filename, "r" );
  char* mnemonic;
  char* line;
  
  //SECOND PASS
  if(input_file == NULL){
    printf("Unable to open file %s\n", filename );
  }else{
    // Read each line into the buffer
    while(fgets(buffer, BUFFER_SIZE, input_file) != NULL ){
      mnemonic = strtok(buffer, " ");
      line = strtok(NULL, "\n" );
      int hash = 0;
      for(int i = 0; i < 4; i++){
        hash += ((int) mnemonic[i]) * (i+1);
      }
      if(mnemonic[strlen(mnemonic) - 1] != '\n'){
        uint32_t result = mnemonicTable[hash](hash, line);
        //TODO:output binary to file
        printf("%i", result);
      }
    }
    if(ferror(input_file) ){
      perror( "The following error occurred" );
    }
    fclose( input_file );
  }

  //CLEAR SymbolTable
  clearSymbolTable();
  return EXIT_SUCCESS;
}
