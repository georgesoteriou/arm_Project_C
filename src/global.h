#include <stdint.h>

uint32_t endianConversion(uint32_t);

struct ARM {
    uint32_t registers[17];
    uint32_t memory[16384];
}arm;

uint32_t fetchCommand;
uint32_t decodeCommand;
uint32_t executeCommand;

void calculateShiftedOperand(uint32_t*, uint32_t);

uint32_t calculateImmediateOperand(uint32_t);

void flagsZN(int32_t);

uint32_t getOffsetWord(uint32_t);

int32_t isWithinBounds(int32_t);
