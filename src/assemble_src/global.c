#include "global.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void initGlobalValues(void){
  
}

uint32_t endianConversion(uint32_t bits) {
  uint32_t mask = (1 << 8) - 1;
  uint32_t result = (bits & mask);
  for(int i = 0; i < 3; i++) {
    result <<= 8;
    bits >>= 8;
    result += (bits & mask);
  }
  return result;
}

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

