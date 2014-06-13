#include "../machine.h"

//! Test un instruction illegal
/*
 * cop==0 c'est un instruction illegal
*/
Instruction text[] = {
//   type                cop	imm     ind     regcond	operand
//-------------------------------------------------------------
    {.instr_immediate =  {0, 	 false, false, 	0, 	0	}},  // 0

};

//! Taille utile du programme
const unsigned textsize = sizeof(text) / sizeof(Instruction);

//! Premier exemple de segment de données initial
Word data[20] = {
    10,  // 0: premier operande
    5,  // 1: second operande
    20, // 2: resultat
    0,  // 3:
};

//! Fin de la zone de données utile
const unsigned dataend = 10;

//! Taille utile du segment de données
const unsigned datasize = sizeof(data) / sizeof(Word);
