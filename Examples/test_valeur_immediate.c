#include "../machine.h"

//! Test une valeur immediate
/*!
 * La valeur immediate de l'operateur STORE doit etre false.
 */

Instruction text[] = {
//   type		 cop	imm	ind	regcond	operand
//-------------------------------------------------------------
    
    {.instr_absolute =  {STORE,  true, false, 	0, 	1	}},  // 4
    
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
