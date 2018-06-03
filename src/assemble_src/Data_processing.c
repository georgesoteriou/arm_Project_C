#include "Data_Processing.h"
#include<string.h>

uint32_t opcodeTable[10] = {4, 2, 3, 0, 1, 12, 13, 8, 9, 10};

uint32_t calcRegister(char* r) {
    int32_t rVal = 0;
    
    while ( (*r) != '\0') {
      rVal = (rVal * 10) + ((*r) - '1');
      r += 1;
    }

    return rVal;
}

uint32_t dataProcessing(int hash, char* str){
  int32_t result = 0;
  //setting cond
  result = 14 << 28;

  //Setting S bit to 1 if it's a test instruction
  if (hash >= 7) {
    result += (1 << 20);
  }

  //setting the opcode
  result += (opcodeTable[hash] << 21);

  if(hash <= 6) {
    //getting rd from the first argument;
   char* rd = strtok(str, " ");
   //getting rid of 'r' from string
   rd += 1;
   //calculating rd value
   result += (calcRegister(rd) << 12);
  }

  //getting 2nd register
  if(hash <= 5) {
    char* rn = strtok(NULL, " ");
    //getting rid of 'r' from string
    rn += 1; 

    //calculating rn value
    result += (calcRegister(rn) << 16);
    
    char* operand = strtok(NULL, " ");
    if((*operand) == '#') {
      //the opperand is an immediate value;
      //set I bit
      result += (1 << 25);
      char* imm = operand + 1;


      int32_t count = 0;
      //immediate value - can only have 2 chars != 0 IN HEX
      //and 8 chars != 0 IN BINARY
      while((*imm) != '\0') {
        if((*imm) != '0') {
          count += 1;
        }
        imm ++;
      }

      if(count > 2) {
        //constant cannot be representer return some error
      } 

      imm = operand + 1;
      
      //somehow process it
    }
  }
 
  
  return 1;
}
