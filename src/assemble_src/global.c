#include "global.h"
#include <stdint.h>
#include <stdlib.h>

void initGlobalValues(void){
  
}

struct SymbolNode* initSymbolTable(){
  struct SymbolNode* symbolTable = (struct SymbolNode*) (malloc( sizeof( struct SymbolNode ) ));
  symbolTable->label = NULL;
  symbolTable->address = 0;
  symbolTable->next = NULL;
  return symbolTable;
}

void addSymbolTable(struct SymbolNode* head, char* label, uint32_t address){
  struct SymbolNode* curr = head;
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

uint32_t getSymbolTable(struct SymbolNode* head, char* label){
  struct SymbolNode* curr = head;
  while(curr != NULL && curr->label != label){
    curr = curr->next;
  }
  if(curr != NULL) {
    return curr->address;
  } else {
    return -1;
  }
}

void clearSymbolTable(struct SymbolNode* head){
  if(head->next != NULL){
    clearSymbolTable(head->next);
  }
  free(head);
}