#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "instruction.h"

//! Forme imprimable des codes operations
const char* cop_names[] = { "ILLOP", "NOP", "LOAD", "STORE", "ADD", "SUB", "BRANCH", "CALL", "RET", "PUSH", "POP", "HALT" };


//! Forme imprimable des conditions
const char* condition_names[] = { "NC", "EQ", "NE", "GT", "GE", "LT", "LE" };

//affiche les operandes des operations d'une instruction inst sous forme intelligible.
void print_operande(Instruction instr) {
    //On récuppére I et X pour choisir ce que l'on affiche
    bool immediate = instr.instr_generic._immediate; // immediate = I
    bool indexed = instr.instr_generic._indexed; // indexed = X

    if (immediate) {
        // si immediate = 1, affiche la valeur imediate
        printf("#%d", instr.instr_immediate._value);
    } else {                
        if (indexed) {
            // Si immediate = 0 et indexed = 1 nous sommes dans le cas d'un adressage indexe
            printf("%+d[",instr.instr_indexed._offset); // Offset sous la forme +/-offset
            printf("R%02d]",instr.instr_indexed._rindex); // Registre pour l'adressage indirect [R..]
        } else {
            // Si immediate = 0 et indexed = 0 : nous sommes dans le cas d'un adressage direct
            printf("@%04x",instr.instr_absolute._address);   
        }
    }
}

//! Impression d'une instruction sous forme lisible (desassemblage)
/*!
 * \param instr l'instruction a imprimer
 * \param addr son adresse
 */
void print_instruction(Instruction instr, unsigned addr) {
    //affiche le nom de l'opération en utilisant le code opération de l'instruction pour le réccupérer dans
    //le tableau cop_names
    printf("%s ", cop_names[instr.instr_generic._cop]);
    int reg = instr.instr_generic._regcond;

    switch (instr.instr_generic._cop) {
        //Pas d'acces memoire :
        case ILLOP:
        case NOP:
        case RET:
        case HALT:
            break;

        //Acces memoire :
        case LOAD:
        case STORE:
        case ADD:
        case SUB:
            //affiche registre d'instr sous forme intelligible.
            printf("R%02d, ", reg);
            //appel de print_operande
            print_operande(instr);
            break;

        case BRANCH:
        case CALL:
            //affiche le code condition d'une instruction sous forme intelligible
            printf("%s ", condition_names[reg]);
            //appel de print_operande
            print_operande(instr);
            break;

        //Acces a la pile :
        case PUSH:
        case POP:
            //appel de print_operande
            print_operande(instr);
            break;
    }
}
