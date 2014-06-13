#include "../machine.h"

//! Test un registor illegal
/*
 * Il y a totalement douze operateurs. Quand on fait cop=12, il y a un erreur.
*/
Instruction text[] = {
//   type                cop	imm     ind     regcond	operand
//-------------------------------------------------------------
    {.instr_immediate =  {LOAD, true, false, 	17, 	2	}},  // 0

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
