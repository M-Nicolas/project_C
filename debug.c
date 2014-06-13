#include <stdio.h>
#include <stdlib.h>
#include "debug.h"

//! Affichage toutes les commandes pour aider

void print_help(){
    printf("Available commands:");
    printf("\t h \t help\n");
    printf("\t c \t continue (exit interactive debug mode)\n");
    printf("\t s \t step by step (next instruction)\n");
    printf("\t RET \t step by step (next instruction)\n");
    printf("\t r \t print registers\n");
    printf("\t d \t print data memory\n");
    printf("\t t \t print text (program) memory\n");
    printf("\t p \t print text (program) memory\n");
    printf("\t m \t print registers and data memory\n");
}

//! Dialogue de mise au point interactive pour l'instruction courante.
/*!
 * Cette fonction gère le dialogue pour l'option \c -d (debug). Dans ce mode,
 * elle est invoquée après l'exécution de chaque instruction.  Elle affiche le
 * menu de mise au point et on exécute le choix de l'utilisateur. Si cette
 * fonction retourne faux, on abandonne le mode de mise au point interactive
 * pour les instructions suivantes et jusqu'à la fin du programme.
 *
 * \param mach la machine/programme en cours de simulation
 * \return vrai si l'on doit continuer en mode debug, faux sinon
 */

bool debug_ask(Machine *pmach){
    char* c=malloc(2*sizeof(char));
        while(true){
            printf("DEBUG? ");
            scanf("%s",c);
            switch (c[0]) {
            case 'h':   // Imprimer les informations
                print_help();
                break;
            case 'c':   // Quit le mode debug
                return false;
            case 's':   // Continue a la commande prochaine
                return true;
            case 'R':   // Rien fait
                break;
            case 'r':   // Imprimer les registres
                print_cpu(pmach);
                break;
            case 'd':   // Imprimer les segments de donnes
                print_data(pmach);
                break;
            case 't':   // Imprimer les segments de texte
            case 'p':   // Imprimer les segments de texte
                print_program(pmach);
                break;
            case 'm':   // Imprimer les segments de donnes et de texte
                print_cpu(pmach);
                print_data(pmach);
                break;
            default:
                printf("Default mode: step by step");
                return true;
           }
    }
    return false;
}

