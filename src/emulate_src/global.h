#include <stdint.h>

#ifndef GLOBAL_H
#define GLOBAL_H

  //17 resigers
  extern const int registerSize;
  //64kB / 32 bit word = 16384 32 bit memory words
  extern const int memorySize;
  //PC is register 15
  extern const int PC;
  //CPSR is register 16
  extern const int CPSR;

  //General Constants
  extern const int nMask;
  extern const int zMask;
  extern const int cMask;
  extern const int vMask;

  extern const int nBit;
  extern const int zBit;
  extern const int cBit;
  extern const int vBit;

  extern const int regAddrLength;
  extern const int opcodeLength;
  extern const int condLength;
  extern const int immediateLength;
  extern const int sdtOffsetLength;
  extern const int operandLength;
  extern const int branchOffsetLength;

  extern const int opcodeStart;
  extern const int condStart;

  extern const int iBit;
  extern const int sBit;
  extern const int lBit;
  extern const int aBit;
  extern const int pBit;
  extern const int uBit;
#endif 

void initGlobalVariables(void);


struct ARM {
  uint32_t registers[17];
  uint32_t memory[16384];
  uint32_t fetchCommand;
  uint32_t decodeCommand;
  uint32_t executeCommand;
}arm;

uint32_t gpioOn;
uint32_t gpioOff;

uint32_t endianConversion(uint32_t);
uint32_t selectBit(uint32_t, uint32_t);
uint32_t selectBits(uint32_t, uint32_t, uint32_t);

void flagsZN(int32_t);

uint32_t getOffsetWord(uint32_t);
void setOffsetWord(uint32_t, uint32_t);

int32_t isWithinBounds(int32_t);
