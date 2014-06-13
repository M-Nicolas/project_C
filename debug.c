#include <stdio.h>
#include "debug.h"

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


bool debug_ask(Machine *pmach){
    char c='a';

        printf("DEBUG? ");
        scanf("%c",&c);

            switch (c) {
            case 'h':
                print_help();
                break;
            case 'c':
                return false;
            case 's':
                return true;
            case 'R':
                break;
            case 'r':
                print_cpu(pmach);
                break;
            case 'd':
                print_data(pmach);
                break;
            case 't':
            case 'p':
                print_program(pmach);
                break;
            case 'm':
                print_cpu(pmach);
                print_data(pmach);
                break;
           }
    return true;
}

