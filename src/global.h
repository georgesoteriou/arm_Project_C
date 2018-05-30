#include <stdint.h>

//GLOBAL VARIABLES
void initGlobalValues(void);
int registerSize;
int memorySize;
int PC;
int CPSR;

uint32_t endianConversion(uint32_t);

struct ARM {
  uint32_t registers[17];
  uint32_t memory[16384];
  uint32_t fetchCommand;
  uint32_t decodeCommand;
  uint32_t executeCommand;
}arm;

uint32_t eofInst;

void flagsZN(int32_t);

uint32_t getOffsetWord(uint32_t);
void setOffsetWord(uint32_t, uint32_t);

int32_t isWithinBounds(int32_t);
