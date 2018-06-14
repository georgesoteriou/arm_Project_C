#include "Single_Data_Transfer.h"
#include "Data_Processing.h"

void setRdField(int32_t *instruction, int32_t Rd){
  (*instruction) += Rd << 12;
}

void setCond(int32_t* instruction){
  (*instruction) += ((uint32_t) condAlways << condStart);
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

char* findExpressionAndSetUFlag(char* addressString, int32_t* instruction){
  /* this function checks only the cases when the register is shifted by
  <#expression> and basically checks if the expression is positive. If it is,
  then it sets the U flag. It also returns the value of the expression*/
  while(!isdigit(*addressString) && (*addressString) != '-'){
    addressString += 1;
  }
  if((*addressString) == '-'){
    addressString += 1;
  } else {
    setUBit(instruction);
  }
  return addressString;
}

void numericalConstant(int32_t Rd, char* addressString, int32_t* instruction){
  addressString = findExpressionAndSetUFlag(addressString, instruction);
  int32_t expression = (int32_t) strtol(addressString, NULL, 0);
  if(expression < 255){
    //less than 0xFF, which is 255 in decimal -> execute normal mov function
    *instruction =  SDTmov(Rd, expression);
    setRdField(instruction, Rd);
  } else {
    //copy the expression at the end of the instructions
    addData(expression);
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
  char* expression = findExpressionAndSetUFlag(addressString, instruction);
  int32_t offset = (int32_t) strtol(expression, NULL, 0);
  (*instruction) += offset;
}

void preIndexingNonShiftedRegister(char* addressString, int32_t* instruction){
  char* expression = findExpressionAndSetUFlag(addressString, instruction);
   //removing the last character, which we know that is ']'
  *(expression + strlen(expression) - 1) = '\0';
  int32_t offset = (int32_t) strtol(expression, NULL, 0);
  (*instruction) += offset;
}

void shiftedRegister(char* addressString, int32_t* instruction){
  setIBit(instruction);
  int32_t Rm = 0, offset = 0;
  if(strchr(addressString, ',')){
    //it means the instruction contains a <shift> as well, not only the Rm
    char* RmString = strtok(addressString, ",");
    RmString = findExpressionAndSetUFlag(addressString, instruction);
    Rm = atoi(RmString);
    offset = Rm;

    char* shiftType = strtok(NULL, " ");
    offset += (shiftID(shiftType)) << shiftStart;

    addressString = strtok(NULL, "");
    addressString += 1;
    addressString = removeSpaces(addressString);
    int32_t amount = atoi(addressString);
    offset += amount << 7;
  } else {
    //it means it's only Rm with an ending ']'
    char* RmString = findExpressionAndSetUFlag(addressString, instruction);
    Rm = atoi(RmString);
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
          shiftedRegister(addressString, instruction);
        }
      }
    } else {
      //pre-indexed register with offset != 0
      preIndexed(instruction);
      addressString += 1;
      addressString = removeSpaces(addressString);
      if(*addressString == '#'){
        //pre-indexed register without shift
        preIndexingNonShiftedRegister(addressString, instruction);
      } else {
      /*pre-indexed instruction using base register Rn and the contents
      of Rm that can be shifted or not*/
      *(addressString + strlen(addressString) - 1) = '\0';
      shiftedRegister(addressString, instruction);
      }
    }
}

void decodeAddressSpecification(int32_t Rd, char* addressString, int32_t* instruction){
  if(*addressString == '='){
    preIndexed(instruction);
    numericalConstant(Rd, addressString, instruction);
  } else {
    /*if the first character is not =, then it is obviously [, so the function
    will check for the other keys & skip the chars [r -> += 2 */
    registerInstruction(addressString + 2, instruction);
  }
}

uint32_t singleDataTransfer(int id, char* str){

  int32_t instruction = 0;

  setCond(&instruction);
  set01bits(&instruction);
  setLBit(&instruction, id);

  /*splitting the instruction before a comma, taking the second character
  (the first one will always be r, because it has the format r_digit) and
  then converting it to int to get the register index*/
  int32_t Rd = atoi((strtok(str, ",")) + 1);
  char* addressString = strtok(NULL, "");

  setRdField(&instruction, Rd);

  addressString = removeSpaces(addressString);

  decodeAddressSpecification(Rd, addressString, &instruction);

  return instruction;
}
