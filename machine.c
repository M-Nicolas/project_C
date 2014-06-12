#include "machine.h"
#include "exec.h"
#include "debug.h"
#include "error.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <fcntl.h>

void load_program(Machine *pmach,
                  unsigned textsize, Instruction text[textsize],
                  unsigned datasize, Word data[datasize],  unsigned dataend) {

    //RaZ des registres
    for(int i = 0 ; i < NREGISTERS ; i++)
        pmach->_registers[i] = 0;

    pmach->_textsize=textsize;
    pmach->_datasize=datasize;
    pmach->_dataend=dataend;
    pmach->_text=text;
    pmach->_data=data;
    pmach->_cc=CC_U;
    pmach->_pc=0;
    pmach->_sp=datasize-1;
}

void read_program(Machine *mach, const char *programfile) {

    int fd=open(programfile,O_RDONLY);
    if (fd<0) {
        printf("Erreur lors de l'ouverture du fichier");
        exit(1);
    }
    unsigned int textsize,datasize,dataend;

    int n = read(fd,&textsize,sizeof(Word));
    if (n!=sizeof(Word)) {
        printf("Erreur de lecture des bits");
        exit(1);
    }
    n = read(fd,&datasize,sizeof(Word));
    if (n!=sizeof(Word)) {
        printf("Erreur de lecture des bits");
        exit(1);
    }
    n = read(fd,&dataend,sizeof(Word));
    if (n!=sizeof(Word)) {
        printf("Erreur de lecture des bits");
        exit(1);
    }
    mach->_textsize=textsize;
    mach->_datasize=datasize;
    mach->_dataend=dataend;

    Instruction* instr = malloc(textsize*sizeof(Instruction));
        n = read(fd,instr,textsize*sizeof(Instruction));
        if (n!=textsize*sizeof(Instruction)) {
            printf("erreur de lecture des bits");
            exit(1);
        }

    Word* data = malloc(datasize*sizeof(Word));
        n = read(fd,data,datasize*sizeof(Word));
        if (n!=datasize*sizeof(Word)) {
            printf("Erreur de lecture des bits");
            exit(1);
        }

    //RaZ des registres
    for(int i = 0 ; i < NREGISTERS ; i++)
        mach->_registers[i] = 0;

    mach->_text=instr;
    mach->_data=data;
    mach->_pc=0;
    mach->_cc=CC_U;
    mach->_sp=datasize-1;
    close(fd);

}

void dump_memory(Machine *pmach) {

    printf("Instruction text[] = {\n");
    for(int i = 0 ; i < pmach->_textsize ; i++)
    {
      printf("0x%08x, ", pmach->_text[i]._raw);
      if (i % 4 == 3)
        putchar('\n');
    }
    if (pmach->_textsize % 4 != 0)
          printf("\n");

    printf("}\n");
    printf("unsigned textsize = %d\n", pmach->_textsize);

    printf("\nWord data[] = {\n");
    //Affichage des données au format binaire:
    for(int i = 0 ; i < pmach->_datasize ; i++)
    {
      printf("0x%08x, ", pmach->_data[i]);
      if (i % 4 == 3)
        printf("\n");
    }
    if (pmach->_datasize % 4 != 0) printf("\n");

    printf("}\n");
    printf("unsigned datasize = %d\n", pmach->_datasize);
    printf("unsigned dataend = %d\n", pmach->_dataend);

    FILE* fd=fopen("dump.prog","w");
    fwrite(&pmach->_textsize,1,sizeof(pmach->_textsize),fd);
    fwrite(&pmach->_datasize,1,sizeof(pmach->_datasize),fd);
    fwrite(&pmach->_dataend,1,sizeof(pmach->_dataend),fd);
    fwrite(pmach->_text,pmach->_textsize,sizeof(Instruction),fd);
    fwrite(pmach->_data,pmach->_datasize,sizeof(Word),fd);
    fclose(fd);

}

void print_program(Machine *pmach) {
    printf("\n*** Impression du programme (instructions) (Textsize= %d) ***\n\n",pmach->_textsize);
    for(int i = 0 ; i < pmach->_textsize ; i++)
      {
        //Affichage du code de l'instruction en hexadecimal
        printf("0x%04x: 0x%08x\t", i, pmach->_text[i]._raw);
        //Affichage de l'instruction
        print_instruction(pmach->_text[i],pmach->_text[i].instr_absolute._address);
        printf("\n");
      }
}

void print_data(Machine *pmach) {
    printf("\n*** DATA Datasize= %d, end= 0x%08x (%d) ***\n\n",pmach->_datasize,pmach->_dataend,pmach->_dataend);
    for (int i=0;i<pmach->_datasize;i++) {
        printf("0x%04x: 0x%08x %d \t",i,pmach->_data[i],pmach->_data[i]);
        if (i%3==0) printf("\n");
    }
    printf("\n");
}


void print_cpu(Machine *pmach) {
    char c;
    switch(pmach->_cc) {
    case CC_U:
        c='U';
        break;
    case CC_Z:
        c='Z';
        break;
    case CC_P:
        c='P';
        break;
    case CC_N:
        c='N';
        break;
    }

    printf("\n*** CPU ***\n");
    printf("PC: 0x%08x CC: %c \n\n",pmach->_pc,c);

    for (int i=0;i<NREGISTERS;i++) {
        printf("R%02d : 0x%08x %d \t",i,pmach->_registers[i],pmach->_registers[i]);
        if (i%3==0) printf("\n");
    }
    printf("\n");
}


void simul(Machine *pmach, bool debug) {
    //Boucle sur les instructions
    while (1) {

        if (pmach->_pc>=pmach->_textsize) {
            error(ERR_SEGTEXT, pmach->_pc - 1);
        }

        trace("Execution de", pmach, pmach->_text[pmach->_pc], pmach->_pc);

        //Condition d'arret du programme
        if (!decode_execute(pmach, pmach->_text[pmach->_pc++])) {
            printf("\\!/ Arrêt du programme \\!/ \n");
            break;
        }
        if (debug) debug = debug_ask(pmach);
    }
}
