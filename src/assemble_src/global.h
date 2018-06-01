#include <stdint.h>

void initGlobalValues(void);

uint32_t endianConversion(uint32_t bits);

struct SymbolNode{
  char* label;
  uint32_t address;
  struct SymbolNode* next;
};

uint32_t currAddress;

struct SymbolNode* SymbolTable;

void initSymbolTable(void);

void addLabel(char*, uint32_t);

int32_t getAddress(char*);

void clearSymbolTable(void);