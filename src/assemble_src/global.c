#include "global.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

const int condStart = 28;
const int opcodeStart = 21;
const int shiftStart = 5;

const int32_t condAlways = 14;

const int32_t wordLength = 32;

char* removeSpaces(char *str) {
  //Remove Spaces
  while((*str) == ' '){
    str++;
  }
  return str;
}

//LDR List
struct LDRNode* LDRTable;

void initLDRTable(void){
  LDRTable = (struct LDRNode*) (malloc( sizeof( struct LDRNode ) ));
  LDRTable->data = 0;
  LDRTable->next = NULL;
}

void addData(uint32_t data){
  struct LDRNode* curr = LDRTable;
  while(curr->data != 0){
    curr = curr->next;
  }
  curr->next = (struct LDRNode*) (malloc( sizeof( struct LDRNode ) ));
  curr->next->data = 0;
  curr->next->next = NULL;
  curr->data = data;
}

uint32_t getData(void){
  struct LDRNode* curr = LDRTable;
  uint32_t data = curr->data;
  LDRTable = LDRTable->next;
  free(curr);
  return data;
}
void clearLDRTableHelper(struct LDRNode* head){
  if(head->next != NULL){
    clearLDRTableHelper(head->next);
  }
  free(head);
}

void clearLDRTable(void){
  if(LDRTable != NULL){
    if(LDRTable->next != NULL){
      clearLDRTableHelper(LDRTable->next);
    }
    free(LDRTable);
  }
}


//Label Symbol linked list
struct SymbolNode* SymbolTable;

void initSymbolTable(void){
  SymbolTable = (struct SymbolNode*) (malloc( sizeof( struct SymbolNode ) ));
  SymbolTable->label = NULL;
  SymbolTable->address = 0;
  SymbolTable->next = NULL;
}

void addLabel(char* label, uint32_t address){
  struct SymbolNode* curr = SymbolTable;
  while(curr->label != NULL){
    curr = curr->next;
  }
  curr->next = (struct SymbolNode*) (malloc( sizeof( struct SymbolNode ) ));
  curr->next->label = NULL;
  curr->next->address = 0;
  curr->next->next = NULL;
  curr->label = malloc(strlen(label) + 1);
  strcpy(curr->label, label);
  curr->address = address;
}

int32_t getAddress(char* label){
  struct SymbolNode* curr = SymbolTable;
  while(curr != NULL && strcmp(curr->label,label) != 0){
    curr = curr->next;
  }
  if(curr != NULL) {
    return curr->address;
  } else {
    return -1;
  }
}

void clearSymbolTableHelper(struct SymbolNode* head){
  if(head->next != NULL){
    clearSymbolTableHelper(head->next);
  }
  free(head->label);
  free(head);
}

void clearSymbolTable(void){
  if(SymbolTable->next != NULL){
    clearSymbolTableHelper(SymbolTable->next);
  }
  free(SymbolTable->label);
  free(SymbolTable);
}


