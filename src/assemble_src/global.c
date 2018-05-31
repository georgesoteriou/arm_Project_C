#include "global.h"
#include <stdint.h>
#include <stdlib.h>

void initGlobalValues(void){
  
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
  curr->label = label;
  curr->address = address;
}

uint32_t getAddress(char* label){
  struct SymbolNode* curr = SymbolTable;
  while(curr != NULL && curr->label != label){
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
  free(head);
}

void clearSymbolTable(void){
  if(SymbolTable->next != NULL){
    clearSymbolTableHelper(SymbolTable->next);
  }
  free(SymbolTable);
}

