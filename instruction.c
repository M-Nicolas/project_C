#include <stdbool.h>
#include <stdint.h>

//! Forme imprimable des codes opérations
const char* cop_names[] = { "ILLOP", "NOP", "LOAD", "STORE", "ADD", "SUB", "BRANCH", "CALL", "RET", "PUSH", "POP", "HALT" };


//! Forme imprimable des conditions
const char* condition_names[] = { "NC", "EQ", "NE", "GT", "GE", "LT", "LE" };

void print_op(Instruction instr);

//! Impression d'une instruction sous forme lisible (désassemblage)
/*!
 * \param instr l'instruction à imprimer
 * \param addr son adresse
 */
void print_instruction(Instruction instr, unsigned addr) {
    printf("%s ", cop_names[instr->instr_generic->_cop]);
    int reg = instr->instr_generic->_regcond;

    switch (instr.instr_generic._cop) {
        //Pas d'accés mémoire :
        case ILLOP:
        case NOP:
        case RET:
        case HALT:
            break;

        //Accés mémoire :
        case LOAD:
        case STORE:
        case ADD:
        case SUB:
            //affiche registre d'instr sous forme intelligible.
            printf("R%02d, ", reg);
            //appel de print_op
            print_op(instr);
            break;

        case BRANCH:
        case CALL:
            //affiche le code condition d'une instruction sous forme intelligible
            printf("%s ", condition_names[reg]);
            //appel de print_op
            print_op(instr);
            break;

        //Accés à la pile :
        case PUSH:
        case POP:
            //appel de print_op
            print_op(instr);
            break;
    }
}

//affiche les opérandes des opérations d'une instruction inst sous forme intelligible.
void print_op(Instruction instr) {
    bool immediate = instr->instr_generic->_immediate; // immediate = I
    bool indexed = instr->instr_generic->_indexed; // indexed = X

    if (immediate) {
        // si immediate = 1, affiche la valeur imédiate
        printf("#%d", instr->instr_immediate->_value);
    } else {                
        if (indexed) {
            // Si immediate = 0 et indexed = 1 nous sommes dans le cas d'un adressage indexé
            printf("%+d[", (int) instr->instr_indexed->_offset); // Offset sous la forme +/-offset
            printf("R%02d]", (int) instr->instr_indexed->_rindex); // Registre pour l'adressage indirect [R..]
        } else {
            // Si immediate = 0 et indexed = 0 : nous sommes dans le cas d'un adressage direct
            printf("@%04x", (int) instr->instr_absolute->_address);   
        }
    }
}

