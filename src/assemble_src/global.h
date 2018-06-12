#include <stdint.h>

#define MULTIPLY_FUNCTION_OFFSET 10
#define SDT_FUNCTION_OFFSET 12
#define BRANCH_FUNCTION_OFFSET 14
#define SPECIAL_FUNCTION_OFFSET 21

extern const int condStart;
extern const int opcodeStart;
extern const int shiftStart;
extern const int32_t condAlways;
extern const int32_t wordLength;


uint32_t currAddress;
uint32_t endOfInst;

char* removeSpaces(char*);

//LDR LINKED LIST
struct LDRNode{
  uint32_t data;
  struct LDRNode* next;
};

void initLDRTable(void);

void addData(uint32_t);

uint32_t getData(void);

void clearLDRTable(void);


//SYMBOL LINKED LIST
struct SymbolNode{
  char* label;
  uint32_t address;
  struct SymbolNode* next;
};

void initSymbolTable(void);

void addLabel(char*, uint32_t);

int32_t getAddress(char*);

void clearSymbolTable(void);




