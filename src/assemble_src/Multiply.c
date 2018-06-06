#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Multiply.h"
#include "global.h"

uint32_t multiply(int hash, char* str){
    uint32_t result = (((int32_t) 14) << 28) + (((int32_t)9) << 4); //Multiply constants
    
    char *rd = strtok(str, ", ");
    uint32_t rdVal = atoi(rd + 1) << 16;
    char *rm = strtok(NULL, ", ");
    uint32_t rmVal = atoi(rm + 1);

    result += rdVal + rmVal;

    if(hash == 10) { //mul
        char *rs = strtok(NULL, "\0");
        uint32_t rsVal = atoi(rs + 1) << 8;

        result += rsVal;
    } else { //mla
        uint32_t aBit = 1 << 21;
        char *rs = strtok(NULL, ", ");
        uint32_t rsVal = atoi(rs + 1) << 8;
        char *rn = strtok(NULL, "\0");
        uint32_t rnVal = atoi(rn + 1) << 12;

        result += aBit + rsVal + rnVal;
    }
    
    return result;
}
