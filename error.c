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
    case ERR_UNKNOWN:   // Une instruction inconnu ( COP>=12 || COP<0 )
        printf("ERROR: UNKNOWN INSTRUCTION at address 0x%x\n", addr);
        exit(1);
    case ERR_ILLEGAL:   // Une instruction illegal ( COP==0 )
        printf("ERROR: ILLEGAL INSTRUCTION at address 0x%x\n", addr);
        exit(1);
    case ERR_CONDITION: // Une condition illegal
        printf("ERROR: ILLEGAL CONDITION at address 0x%x\n", addr);
        exit(1);
    case ERR_IMMEDIATE: // Une condition illegal ( I/X != true/false comme prevu)
        printf("ERROR: FORBIDDEN VALUE at address 0x%x\n", addr);
        exit(1);
    case ERR_SEGTEXT:   // Le numero de registre est trop leve
        printf("ERROR: TEXT SEGMENT VIOLATION at address 0x%x\n", addr);
        exit(1);
    case ERR_SEGDATA:   // Le segment de data
        printf("ERROR: DATA SEGMENT VIOLATION at address 0x%x\n", addr);
        exit(1);
    case ERR_SEGSTACK:  // On push ou pull trop dans une pile
        printf("ERROR: STACK SEGMENT VIOLATION at address 0x%x\n", addr);
        exit(1);
    default:
        exit(0);
    }
    exit(0);
}

