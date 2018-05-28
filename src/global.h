#include <stdint.h>

int32_t endianConversion(int32_t);

struct ARM {
    int32_t registers[17];
    int32_t memory[16384];
}arm;

uint32_t fetchCommand;
uint32_t decodeCommand;
uint32_t executeCommand;

void calculateShiftedOperand(int32_t*, int32_t);

int32_t calculateImmediateOperand(int32_t);

void flagsZN(uint32_t);
