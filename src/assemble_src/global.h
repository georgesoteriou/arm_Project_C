#include <stdint.h>

void initGlobalValues(void);

struct SymbolNode{
  char* label;
  uint32_t address;
  struct SymbolNode* next;
};

struct SymbolNode* SymbolTable;

void initSymbolTable(void);

void addSymbolTable(char*, uint32_t);

uint32_t getSymbolTable(char*);

void clearSymbolTable(void);