#include <stdint.h>

void initGlobalValues(void);

struct SymbolNode{
  char* label;
  uint32_t address;
  struct SymbolNode* next;
};

struct SymbolNode* SymbolTable;

struct SymbolNode* initSymbolTable();

void addSymbolTable(struct SymbolNode* , char*, uint32_t);

uint32_t getSymbolTable(struct SymbolNode* , char* );

void clearSymbolTable(struct SymbolNode*);