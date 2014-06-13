/*
 *Afficher les informations de l'erreur
*/
#include<stdio.h>
#include "error.h"

#define MAX 100
/*
* Afficher un warning:
* \param warn code du warning
* \param addr adresse de l'erreur
*/
void warning(Warning warn, unsigned addr){     	
	printf("WARNING: HALT reached at address 0x%x\n", addr);
}

/*
* Afficher un erreur:
* \param err code de l'erreur
* \param addr adresse de l'erreur
*/
void error(Error err, unsigned addr){
    switch (err) {
    case ERR_NOERROR:
        printf("ERROR: NO ERROR at address 0x%x\n", addr);
        break;
    case ERR_UNKNOWN:
        printf("ERROR: UNKNOWN INSTRUCTION at address 0x%x\n", addr);
        break;
    case ERR_ILLEGAL:
        printf("ERROR: ILLEGAL INSTRUCTION at address 0x%x\n", addr);
        break;
    case ERR_CONDITION:
        printf("ERROR: ILLEGAL CONDITION at address 0x%x\n", addr);
        break;
    case ERR_IMMEDIATE:
        printf("ERROR: FORBIDDEN VALUE at address 0x%x\n", addr);
        break;
    case ERR_SEGTEXT:
        printf("ERROR: TEXT SEGMENT VIOLATION at address 0x%x\n", addr);
        break;
    case ERR_SEGDATA:
        printf("ERROR: DATA SEGMENT VIOLATION at address 0x%x\n", addr);
        break;
    case ERR_SEGSTACK:
        printf("ERROR: STACK SEGMENT VIOLATION at address 0x%x\n", addr);
        break;
    default:
        break;
    }
    exit(0);
}

