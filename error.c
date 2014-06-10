#include<stdio.h>

#include "error.h"

#define MAX 100
void warning(Warning warn, unsigned addr){
    if(warn==WARN_HALT){
        printf("WARNING: HALT reached at address 0x%x\n", addr);
    }
}

void error(Error err, unsigned addr){
    switch (err) {
    case 0:
        printf("ERROR: NO ERROR at address 0x%x\n", addr);
        break;
    case 1:
        printf("ERROR: UNKNOWN INSTRUCTION at address 0x%x\n", addr);
        break;
    case 2:
        printf("ERROR: ILLEGAL INSTRUCTION at address 0x%x\n", addr);
        break;
    case 3:
        printf("ERROR: ILLEGAL CONDITION at address 0x%x\n", addr);
        break;
    case 4:
        printf("ERROR: FORBIDDEN VALUE at address 0x%x\n", addr);
        break;
    case 5:
        printf("ERROR: TEXT SEGMENT VIOLATION at address 0x%x\n", addr);
        break;
    case 6:
        printf("ERROR: DATA SEGMENT VIOLATION at address 0x%x\n", addr);
        break;
    case 7:
        printf("ERROR: STACK SEGMENT VIOLATION at address 0x%x\n", addr);
        break;
    default:
        break;
    }
    exit(0);
}

