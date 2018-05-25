#include <stdint.h>

struct ARM {
    int32_t registers[17];
    int32_t memory[16384];
}arm;

void calculateShiftedOperand(int32_t*);

void calculateImmediateOperand(int32_t*);

