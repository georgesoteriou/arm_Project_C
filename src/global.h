#include <stdint.h>

int32_t endianConversion(int32_t);

struct ARM {
    int32_t registers[17];
    int32_t memory[16384];
}arm;

int32_t fetchCommand;
int32_t decodeCommand;
int32_t executeCommand;

void calculateShiftedOperand(int32_t*, int32_t);

int32_t calculateImmediateOperand(int32_t);

void flagsZN(int32_t);
