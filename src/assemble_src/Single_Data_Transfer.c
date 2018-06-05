#include "Single_Data_Transfer.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "global.h"
#include <ctype.h>

void numericalConstant(char* addressString, int32_t* instruction){
  int32_t address = (int32_t) strtol(addressString, NULL, 0);
  fprintf(stdout, "constant value: %d\n", address);
}

void preIndexed(int32_t* instruction){
  (*instruction) += 1 << 24;
}

void registerInstruction(char* addressString, int32_t* instruction){
    /*this functions find Rn and then checks which kind of operation it is,
    pre-indexed or post-indexed*/
    int32_t count = 1;
    char* separators = "], \n";
    //check in the addressString until found the first separator
    while(!strchr(separators, *(addressString + count))){
      count++;
    }
    char* RnString = malloc(sizeof(char) * (count));
    strncpy(RnString, addressString, count);
    int32_t Rn = atoi(RnString);
    fprintf(stdout, "RnString: %s\n", RnString);
    free(RnString);
    int32_t offset = 0;
    //skip Rn and the separator
    addressString += count;
    //int32_t offset = 0;
    while(*addressString == ' '){
      addressString++;
    }
    if(*addressString == ']'){
      //pre-indexed register with offset 0 or post-indexed register
      while(*addressString == ' '){
        addressString++;
      }
      if(*(addressString + 1) != ','){
        //pre-indexed register with offset 0
        offset = 0;
        preIndexed(instruction);
      } else {
        //post-indexed register
        //fprintf(stdout, "got here 3\n");
        while(!isdigit(*addressString)){
          addressString++;
        }
        offset = atoi(addressString);
        fprintf(stdout, "offset: %d\n", offset);
      }
    } else {
      //pre-indexed register with offset != 0
      while(!isdigit(*addressString)){
        addressString++;
      }
      //copy the content, without the last character (it is ']')
      char* offsetString = malloc(sizeof(char) * (strlen(addressString) - 1));
      strncpy(offsetString, addressString, strlen(addressString) - 1);
      offset = atoi(offsetString);
      free(offsetString);
      fprintf(stdout, "lol offset: %d\n", offset);
      preIndexed(instruction);
    }
    (*instruction) += Rn << 16;
    (*instruction) += offset;
}

void decodeAddressSpecification(char* addressString, int32_t* instruction){
  if(*addressString == '='){
    /*numeric constant, so ignore the first character and convert from
    a hex string to int*/
    numericalConstant(addressString + 1, instruction);
    preIndexed(instruction);
    addressString += 1;
  } else {
    /*if the first character is not =, then it is obviously [, so the function
    will check for the other keys & skip the chars [r -> += 2 */
    registerInstruction(addressString + 2, instruction);
  }
}

void setLBit(int32_t *instruction, int id){
  if(id - SDT_FUNCTION_OFFSET == 0){
    (*instruction) = 1 << 20;
  }
}

char* splitInstruction(int32_t *Rd, char* str){
  /*splitting the instruction before a comma, taking the second character
  (the first one will always be r, because it has the format r_digit) and
  then converting it to int to get the register index*/
  *Rd = atoi((strtok(str, ",")) + 1);
  return strtok(NULL, "");
}

void setRdField(int32_t *instruction, int32_t Rd){
  (*instruction) += Rd << 12;
}

uint32_t singleDataTransfer(int id, char* str){

  int32_t instruction = 1 << 26, Rd = 0;
  
  setLBit(&instruction, id);

  char* addressString = splitInstruction(&Rd, str);

  setRdField(&instruction, Rd);

  printf("Register value:%d, address:%s\n", Rd, addressString);
  
  decodeAddressSpecification(addressString + 1, &instruction);  

  return instruction;
}

