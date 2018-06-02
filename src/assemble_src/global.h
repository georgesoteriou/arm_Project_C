#include <stdint.h>

#define MULTIPLY_FUNCTION_OFFSET 10
#define SDT_FUNCTION_OFFSET 12
#define BRANCH_FUNCTION_OFFSET 14
#define SPECIAL_FUNCTION_OFFSET 21

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