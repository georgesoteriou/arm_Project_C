#include "Multiply.h"

#define MUL_CONST (9 << 4)
#define RD 16
#define RS 8
#define RN 12
#define A_BIT (1 << 21)

uint32_t multiply(int hash, char *str){
    //set the constants, cond and the 1001 field
    uint32_t result = (condAlways << condStart) + MUL_CONST; 
    
    char *rd = strtok(str, ", ");
    uint32_t rdVal = atoi(rd + 1) << RD;
    char *rm = strtok(NULL, ", ");
    uint32_t rmVal = atoi(rm + 1);

    result += rdVal + rmVal;

    if(hash == 10) { //mul
        char *rs = strtok(NULL, "\0");
        uint32_t rsVal = atoi(rs + 1) << RS;

        result += rsVal;
    } else { //mla
        char *rs = strtok(NULL, ", ");
        uint32_t rsVal = atoi(rs + 1) << RS;
        char *rn = strtok(NULL, "\0");
        uint32_t rnVal = atoi(rn + 1) << RN;

        result += A_BIT + rsVal + rnVal;
    }
    
    return result;
}
