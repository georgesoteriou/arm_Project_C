#include "Single_Data_Transfer.h"
#include "Data_Processing.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "global.h"
#include <ctype.h>

//TO DO: CHECK IN SHIFTED REGISTERS IF THE FIRST ONE IS NEGATIVE FOR BOTH CASES
//TO DO: REWRITE THE MALLOC THINGY WITH '\0' IN THE END
//TO DO: MAKE A FUNCTION FOR CHECK NEGATIVE
//TO DO: REMOVE NASTY ']' FOR THE SECOND CASE
//TO DO: SEE THE SEPARATORS THING AND MAKE IT WITH STRTOK
//TO DO: MAKE FUNCTION FOR SPLITTING THE TEXT
//TO DO: GET RID OF MAGIC NUMBERS, REFACTOR EVEN MORE IF POSSIBLE

void setRdField(int32_t *instruction, int32_t Rd){
  (*instruction) += Rd << 12;
}

void setCond(int32_t* instruction){
  (*instruction) += ((uint32_t) 14 << 28);
}

void setUBit(int32_t* instruction){
  (*instruction) +=  1 << 23;
}

void set01bits(int32_t* instruction){
  (*instruction) += 1 << 26;
}

void preIndexed(int32_t* instruction){
  (*instruction) += 1 << 24;
}

void setIBit(int32_t* instruction){
  (*instruction) += 1 << 25;
}

void setLBit(int32_t *instruction, int id){
  if(id - SDT_FUNCTION_OFFSET == 0){
    (*instruction) += (((int32_t) 1) << 20);
  }
}

void setRnField(int32_t* instruction, int32_t Rn){
  (*instruction) += Rn << 16;
}

int32_t SDTmov(int32_t Rd, int32_t address){
  int32_t instruction = (((int32_t) 14) << 28) + (((int32_t) 1) << 25) + (((int32_t) 13) << 21);
  instruction += address;
  return instruction;
}

void numericalConstant(int32_t Rd, char* addressString, int32_t* instruction){

  int32_t address = (int32_t) strtol(addressString, NULL, 0);
  if(address < 255){
    *instruction =  SDTmov(Rd, address);
    setRdField(instruction, Rd);
  } else {
    addData(address);
    int32_t Rn = 15;
    (*instruction) += (Rn << 16);
    int32_t offset = 0;
    int32_t PC = currAddress + 8;
    offset = endOfInst - PC;
    endOfInst += 4;
    (*instruction) += offset;
  }
}

void baseRegisterOffsetZero(int32_t* instruction){
  setUBit(instruction);
  preIndexed(instruction);
}

void postIndexingNonShiftedRegister(char* addressString, int32_t* instruction){
  while(!isdigit(*addressString) && (*addressString) != '-'){
    addressString++;
  }
  if((*addressString) == '-'){
    addressString++;
  } else {
    setUBit(instruction);
  }
  int32_t offset = (int32_t) strtol(addressString, NULL, 0);
  (*instruction) += offset;
}

void preIndexingNonShiftedRegister(char* addressString, int32_t* instruction){
  while(!isdigit(*addressString) && (*addressString) != '-'){
    addressString++;
  }
  if((*addressString) == '-'){
    addressString++;
  } else {
    setUBit(instruction);
  }
   //copy the content, without the last character (it is ']')
  char* offsetString = malloc(sizeof(char) * (strlen(addressString) - 1));
  strncpy(offsetString, addressString, strlen(addressString) - 1);
  int32_t offset = (int32_t) strtol(offsetString, NULL, 0);
  free(offsetString);
  (*instruction) += offset;
}

void shiftedRegister(char* addressString, int32_t* instruction){
  setIBit(instruction);
  int32_t Rm = 0, offset = 0;
  if(strchr(addressString, ',')){
    //it means the instruction contains a shift as well
    char* RmString = strtok(addressString, ",");
    RmString = removeSpaces(RmString);
    if((*RmString) == '-'){
      RmString++;
    } else {
      setUBit(instruction);
    }
    Rm = atoi(RmString + 1);
    //last 4 bits of the offset are Rm
    offset = Rm;
    char* shiftType = strtok(NULL, " ");
    //bits 5 and 6 are the shiftType
    offset += (shiftID(shiftType)) << 5;
    addressString = strtok(NULL, "");
    addressString++;
    addressString = removeSpaces(addressString);
    char* amountString = malloc(sizeof(char) * (strlen(addressString) - 1));
    strncpy(amountString, addressString, strlen(addressString) - 1);
    int32_t amount = atoi(amountString);
    free(amountString);
    //the bits 11-7 of the offset are the amount
    offset += amount << 7;
  } else {
    //it means it's only Rm with an ending ']'
    if(*addressString == '-'){
      addressString++;
    } else {
      setUBit(instruction);
    }
    addressString = removeSpaces(addressString);
    //jumping over the R
    addressString++;
    char* RmString = malloc(sizeof(char) * (strlen(addressString) - 1)); 
    strncpy(RmString, addressString, strlen(addressString) - 1);
    Rm = atoi(RmString);
    free(RmString);
    offset = Rm;
  }
(*instruction) += offset;
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
    char* RnString = malloc(sizeof(char) * count);
    strncpy(RnString, addressString, count);
    int32_t Rn = atoi(RnString);
    free(RnString);
    setRnField(instruction, Rn);
    //skip Rn and the separator
    addressString += count;

    addressString = removeSpaces(addressString);
    if(*addressString == ']'){
      //pre-indexed register with offset 0 or post-indexed register
      addressString += 1;
      addressString = removeSpaces(addressString);
      if(*(addressString) != ','){
        //pre-indexed register with offset 0
        baseRegisterOffsetZero(instruction);
      } else {
        //post-indexed register - might be shifted or nonshifted
        addressString += 1;
        addressString = removeSpaces(addressString);
        if((*addressString) == '#'){
          postIndexingNonShiftedRegister(addressString, instruction);
        } else {
          //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
          strcat(addressString, "]");
          shiftedRegister(addressString, instruction);
        }
      }
    } else {
      //pre-indexed register with offset != 0
      preIndexed(instruction);
      addressString++;
      addressString = removeSpaces(addressString);
      if(*addressString == '#'){
        //pre-indexed register without shift
        preIndexingNonShiftedRegister(addressString, instruction);
      } else {
      /*pre-indexed instruction using base register Rn and the contents
      of Rm that can be shifted or not*/
      shiftedRegister(addressString, instruction);
      }
    }
}

void decodeAddressSpecification(int32_t Rd, char* addressString, int32_t* instruction){
  if(*addressString == '='){
    /*numeric constant, so ignore the first character and convert from
    a hex string to int*/
    preIndexed(instruction);
    if(*(addressString + 1) == '-'){
      addressString += 2;
    } else {
      setUBit(instruction);
      addressString += 1;
    }
    numericalConstant(Rd, addressString, instruction);
  } else {
    /*if the first character is not =, then it is obviously [, so the function
    will check for the other keys & skip the chars [r -> += 2 */
    registerInstruction(addressString + 2, instruction);
  }
}

char* splitInstruction(int32_t *Rd, char* str){
  /*splitting the instruction before a comma, taking the second character
  (the first one will always be r, because it has the format r_digit) and
  then converting it to int to get the register index*/
  *Rd = atoi((strtok(str, ",")) + 1);
  return strtok(NULL, "");
}

uint32_t singleDataTransfer(int id, char* str){

  int32_t instruction = 0;

  setCond(&instruction);
  set01bits(&instruction);
  setLBit(&instruction, id);

  int32_t Rd = 0;

  char* addressString = splitInstruction(&Rd, str);

  setRdField(&instruction, Rd);

  addressString = removeSpaces(addressString);
  
  decodeAddressSpecification(Rd, addressString, &instruction);  

  return instruction;
}

