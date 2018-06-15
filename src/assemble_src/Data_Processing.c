#include "Data_Processing.h"

//there are 10 types of data processing instructions
uint32_t opcodeTable[10] = {
  4,    //opcode = 0100     hash = 0    
  2,    //opcode = 0010     hash = 1                   
  3,    //opcode = 0011     hash = 2     
  0,    //opcode = 0000     hash = 3    
  1,    //opcode = 0001     hash = 4      
  12,   //opcode = 1100     hash = 5         
  13,   //opcode = 1101     hash = 6      
  8,    //opcode = 1000     hash = 7  
  9,    //opcode = 1001     hash = 8  
  10    //opcode = 1010     hash = 9   
};

//returns 1 for hex and 0 for binary
int checkImmType(char *imm) {
  if(imm + 1 != '\0') {
    if((*imm) == '0' && (*(imm + 1)) == 'x') {
      return 1;
    } else {
      return 0;
    }
  } else {
    return 0;
  }
}

int32_t rotateRight(int32_t value, int bits) {
  return (value >> bits) | (value << (wordLength - bits));
}

int32_t rotateLeft(int32_t value, int bits) {
  return (value << bits) | (value >> (wordLength - bits));
}

//returns 0 if decimal is not valid
//the decimal is not valid if it cannot be
//represented as an 8 bit rotated immediate
//this means that by using an 8 bit mask and
//rotating it, we should be able to find our 8 bit
//rotated immediate.
//If it is not found, the functions returns 0.
int isValid(int32_t imm) {
  int32_t mask = (1 << 8) - 1;
  int count = 0;
  
  //only 15 possible rotations, 
  for(int i = 0; i < 15; i ++) {
    if((imm - (imm & mask)) == 0) {
      count++;
    }

    mask = rotateRight(mask, 2);
  }

  if(count == 0) {
    return 0;
  } else {
    return 1;
  }
}

//there are 4 possible shifts
const char *shiftTable[4] = {"lsl", "lsr", "asr", "ror"};

int shiftID(char *shift){
  for(int i = 0; i < 4; i++){
    if (strcmp(shiftTable[i], shift) == 0) {
      return i;  
    }
  }
  exit(-1);
}

void processOperand(char *operand, int32_t *result) {
  if((*operand) == '#') {
    //the opperand is an immediate value;
    //set I bit
    (*result) += (((int32_t) 1) << 25);
    char *imm = operand + 1;

    int32_t constant;
    
    if(checkImmType(imm) == 1) {
      constant = strtol(imm + 2, &imm, 16);      
    } else {
      constant = (int32_t) atoi(imm);
    }

    if(isValid(constant) == 0) {
      //error
      exit(-1);
    } else {
      int32_t mask = (1 << 8) - 1;
      int rotationNum = 0;
      //rotate max 15 * 2
      while((constant & mask) - constant != 0)  {
        rotationNum++;
        mask = rotateRight(mask, 2);
      }
      //rotationNum now holds bits 8 - 11
      (*result) += (rotationNum << 8);

      //constant has to be rotated to occupy the first
      //8 bits of result
      (*result) += rotateLeft(constant, 2 * rotationNum);  
    }
  } else {
    //the operand is a register
    operand++;

    char *checkComma = strchr(operand, ',');
    if(checkComma == NULL) {
      (*result) += (int32_t) atoi(operand);
    } else {
      char *r = strtok(operand, ",");
      r = removeSpaces(r);
      (*result) += (int32_t) atoi(r);
      
      //getting shift type
      char *shiftType = strtok(NULL, " ");
      shiftType = removeSpaces(shiftType);
      int32_t shift = shiftID(shiftType);
      (*result) += (shift << shiftStart);

      //getting shift ammount
      char *constant = strtok(NULL, "\0");
      constant = removeSpaces(constant);
      if((*constant) == '#') {
        //register is shifted by constant ammount
        int32_t num;
        if(checkImmType(constant + 1) == 0) {
           num = (atoi(constant + 1)) << 7;
        } else {
           num = ((strtol(constant + 3, NULL, 16))) << 7;
        }
        (*result) += num;
      } else {
        //register is shifted by the bottom byte of a register
        (*result) += (1 << 4);
        int32_t rs = atoi(constant + 1) << 8;
        (*result) += rs;
      }
      
    }

  }
}

int32_t getRd(char *rd){
  //getting rid of 'r' from string
  rd += 1;
  //shift rd value to position in result
  return (((int32_t) atoi(rd)) << 12);
}

int32_t getRn(char *rn){
  //getting rid of 'r' from string
  rn += 1; 
  //shift rn value to position in result
  return (((int32_t) atoi(rn)) << 16);
}

int32_t get3parameters(char *str){
  int32_t result = 0;
  //getting rd from the first argument;
  char *rd = strtok(str, ",");
  rd = removeSpaces(rd);
  result += getRd(rd);

  //getting rn from the second argument;
  char *rn = strtok(NULL, ",");
  rn = removeSpaces(rn);
  result += getRn(rn);
  
  //getting operand from the third argument;
  char *operand = strtok(NULL, "\0");
  operand = removeSpaces(operand);
  processOperand(operand, &result);
  return result;
}

int32_t add(char *str){
  return get3parameters(str);
}

int32_t sub(char *str){
  return get3parameters(str);
}

int32_t rsb(char *str){
  return get3parameters(str);
}

int32_t and(char *str){
  return get3parameters(str);
}

int32_t eor(char *str){
  return get3parameters(str);
}

int32_t orr(char *str){
  return get3parameters(str);
}

int32_t mov(char *str){
  int32_t result = 0;
  //getting rd from the first argument;
  char *rd = strtok(str, ",");
  rd = removeSpaces(rd);
  result += getRd(rd);

  //get operand
  char *operand = strtok(NULL, "\0");
  operand = removeSpaces(operand);
  processOperand(operand, &result);
  return result;
}

int32_t tst(char *str){
  int32_t result = 0;
  //set bit 20
  result += (1 << 20);

  char *rn = strtok(str, ",");
  rn = removeSpaces(rn);
  result += getRn(rn);
  
  char *operand = strtok(NULL, "\0");
  operand = removeSpaces(operand);
  processOperand(operand, &result);
  return result;
}

int32_t teq(char *str){
  int32_t result = 0;
  //set bit 20
  result += (1 << 20);

  char *rn = strtok(str, ",");
  rn = removeSpaces(rn);
  result += getRn(rn);
  
  char *operand = strtok(NULL, "\0");
  operand = removeSpaces(operand);
  processOperand(operand, &result);
  return result;
}

int32_t cmp(char *str){
  int32_t result = 0;
  //set bit 20
  result += (1 << 20);

  char *rn = strtok(str, ",");
  rn = removeSpaces(rn);
  result += getRn(rn);
  
  char *operand = strtok(NULL, "\0");
  operand = removeSpaces(operand);
  processOperand(operand, &result);
  return result;
}

typedef int32_t (*data_f)(char*);

const data_f data_fTable[10] = {
 add,
 sub,
 rsb,
 and,
 eor,
 orr,
 mov,
 tst,
 teq,
 cmp
};


uint32_t dataProcessing(int hash, char *str){
  int32_t result = 0;
  str = removeSpaces(str);
  //setting cond
  result = ((int32_t) 14) << 28;
  //setting the opcode
  result += (int32_t) (opcodeTable[hash] << 21);
  return result + data_fTable[hash](str);
}
