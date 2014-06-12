/*!
 * \file exec.c
 * \brief Exécution d'une instruction.
 */

#include "exec.h"
#include "error.h"
#include <stdio.h>

/*\
 * \fn void check_stack(Machine *pmach, unsigned ad_Data, unsigned ad_Instr)
 * \brief Vérifie qu'on reste bien dans la pile
 * \param pmach la machine/programme en cours d'exécution
 * \param ad_Instr l'adresse de l'instruction en cours d'execution
 * \param ad_Data l'adresse de la donnée dont on a besoin
 */
void check_stack(Machine *pmach, unsigned ad_Data, unsigned ad_Instr) {
	if (ad_Data < pmach->_dataend || ad_Data >= pmach->_datasize)
		error(ERR_SEGSTACK, ad_Instr);
}

/*\
 * \fn void check_overflow(Machine *pmach, unsigned ad_Data, unsigned ad_Instr)
 * \brief Vérifie qu'on est bien dans le registre de données (pas d'overflow)
 * \param pmach la machine/programme en cours d'exécution
 * \param ad_Instr l'adresse de l'instruction en cours d'execution
 * \param ad_Data l'adresse de la donnée dont on a besoin
 */
void check_overflow(Machine *pmach, unsigned ad_Data, unsigned ad_Instr) {
	if (ad_Data > pmach->_datasize)
		error(ERR_SEGDATA, ad_Instr);
}

/*\
 * \fn void check_not_immediate(Instruction instr, unsigned addr)
 * \brief Vérifie que l'instruction n'est pas codée en immédiate
 * \param instr l'instruction à exécuter
 * \param addr adresse de l'instruction
 * renvoie une erreur si l'instruction est adressée en immédiat
 */void check_not_immediate(Instruction instr, unsigned addr) {
	if (instr.instr_generic._immediate) {
		error(ERR_IMMEDIATE, addr);
	}
}

/*\
 * \fn unsigned int get_adress(Machine *pmach, Instruction instr)
 * \brief Récupère l'adresse suivant si l'instruction suit un adressage direct ou indexé (avec un deplacement)
 * \param pmach le programme en cours d'execution
 * \param instr l'instruction qu'on est en train de lire
 * \return la valeur de l'adresse absolue ou après le déplacement effectué
 */
unsigned int get_adress(Machine *pmach, Instruction instr) {
	if (instr.instr_generic._indexed) { // si l'adressage est indexé on effectue le deplacement et on récupère l'adresse
		return pmach->_registers[instr.instr_indexed._rindex]
				+ instr.instr_indexed._offset; // on renvoie la valeur du registre plus la valeur de déplacement contenue dans l'offset
	}
	return instr.instr_absolute._address; // sinon on récupère l'adresse absolue
}

/*\
 * \fn void refresh_condition(Machine *pmach, unsigned int regcond)
 * \brief après chaque modification du contenu des registres on remet les conditions à jour
 * \param pmach la machine/programme en cours d'exécution
 * \param regcond contenu du registre
 */
void refresh_condition(Machine *pmach, unsigned int regcond) {
	if (regcond < 0) {
		pmach->_cc = CC_N;
	} else if (regcond == 0) {
		pmach->_cc = CC_Z;
	} else {
		pmach->_cc = CC_P;
	}
}

/*\
 * \fn bool check_condition(Machine *pmach, Instruction instr, unsigned addr)
 * \brief Affecte pour chaque condition la valeur que doit contenir le code condition de la machine
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction à exécuter
 * \param addr adresse de l'instruction
 * \return true
 */bool check_condition(Machine *pmach, Instruction instr, unsigned addr) {
	switch (instr.instr_generic._regcond) {
	case NC:
		return true;
	case EQ:
		return (pmach->_cc == CC_Z);
	case NE:
		return (pmach->_cc != CC_Z);
	case GT:
		return (pmach->_cc == CC_P);
	case GE:
		return (pmach->_cc == CC_P || pmach->_cc == CC_Z);
	case LT:
		return (pmach->_cc == CC_N);
	case LE:
		return (pmach->_cc == CC_N || pmach->_cc == CC_Z);
	default:
		error(ERR_CONDITION, addr);
		return false;
	}
}

/*\
 * \fn bool store(Machine *pmach, Instruction instr, unsigned addr)
 * \brief Décodage et exécution de l'instruction STORE
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction à exécuter
 * \param addr adresse de l'instruction
 * \return true
 */bool store(Machine *pmach, Instruction instr, unsigned addr) {
	unsigned ad_Data;
	check_not_immediate(instr, addr);
	ad_Data = get_adress(pmach, instr);
	check_overflow(pmach, ad_Data, addr);
	pmach->_data[ad_Data] = pmach->_registers[instr.instr_generic._regcond]; // Data[Addr] <- R
	return true;
}

/*\
 * \fn bool modify_register(Machine *pmach, Instruction instr, unsigned addr)
 * \brief Décodage et exécution de l'instruction ADD
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction à exécuter
 * \param addr adresse de l'instruction
 * \return true
 */bool modify_register(Machine *pmach, Instruction instr, unsigned addr, int cop) {
	unsigned ad_Data;
	if (cop == 0) { // instruction LOAD
		if (instr.instr_generic._immediate) { // si adressage immédiat
			pmach->_registers[instr.instr_generic._regcond] =
					instr.instr_immediate._value; // R <- Val
		} else {
			ad_Data = get_adress(pmach, instr);
			check_overflow(pmach, ad_Data, addr);
			pmach->_registers[instr.instr_generic._regcond] =
					pmach->_data[ad_Data]; // R <- Data[Addr]
		}
	} else { // instruction ADD et SUB; si cop == -1 -> SUB, si cop == 1 -> ADD
		if (instr.instr_generic._immediate) { // Si adressage immédiat
			pmach->_registers[instr.instr_generic._regcond] +=
					instr.instr_immediate._value * cop; // R <- (R) + Val
		} else {
			ad_Data = get_adress(pmach, instr);
			check_overflow(pmach, ad_Data, addr);
			pmach->_registers[instr.instr_generic._regcond] +=
					pmach->_data[ad_Data] * cop; // (R) <- R + Data[Addr]
		}
	}
	refresh_condition(pmach, pmach->_registers[instr.instr_generic._regcond]);

	return true;
}



/*\
 * \fn bool call_branch(Machine *pmach, Instruction instr, unsigned addr)
 * \brief Décodage et exécution des instructions CALL et BRANCH
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction à exécuter
 * \param addr adresse de l'instruction
 * \return true
 */bool call_branch(Machine *pmach, Instruction instr, unsigned addr, Code_Op cop) {
	check_not_immediate(instr, addr);
	if (check_condition(pmach, instr, addr)) {
		if(cop == CALL){
			check_overflow(pmach, pmach->_sp, addr);
			pmach->_data[pmach->_sp] = pmach->_pc; // Data[SP] <- PC
			check_stack(pmach, pmach->_sp--, addr); // on décrémente sp et verifie qu'on ne sort pas de la pile
		}
		pmach->_pc = get_adress(pmach, instr); // PC <- Addr
	}
	return true;
}

/*\
 * \fn bool ret(Machine *pmach, Instruction instr, unsigned addr)
 * \brief Décodage et exécution de l'instruction RET
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction à exécuter
 * \param addr adresse de l'instruction
 * \return true
 */bool ret(Machine *pmach, Instruction instr, unsigned addr) {
	check_overflow(pmach, pmach->_sp++, addr); // on incrémente sp et verifie qu'on ne sort pas de la pile
	pmach->_pc = pmach->_data[pmach->_sp]; // PC <- Data[SP]
	return true;
}

/*\
 * \fn bool push(Machine *pmach, Instruction instr, unsigned addr)
 * \brief Décodage et exécution de l'instruction PUSH
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction à exécuter
 * \param addr adresse de l'instruction
 * \return true
 */bool push(Machine *pmach, Instruction instr, unsigned addr) {
	unsigned ad_Data;
	if (instr.instr_generic._immediate) { // si adressage immédiat
		check_overflow(pmach, pmach->_sp, addr);
		pmach->_data[pmach->_sp] = instr.instr_immediate._value; // Data[SP] <- Val
	} else {
		check_overflow(pmach, pmach->_sp, addr);
		ad_Data = get_adress(pmach, instr);
		check_overflow(pmach, ad_Data, addr);
		pmach->_data[pmach->_sp] = pmach->_data[ad_Data]; // Data[SP] <- Data[Addr]
	}
	check_stack(pmach, pmach->_sp--, addr); // on décrémente sp et verifie qu'on ne sort pas de la pile
	return true;
}

/*\
 * \fn bool pop(Machine *pmach, Instruction instr, unsigned addr)
 * \brief Décodage et exécution de l'instruction POP
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction à exécuter
 * \param addr adresse de l'instruction
 * \return true
 */bool pop(Machine *pmach, Instruction instr, unsigned addr) {
	unsigned ad_Data;

	check_not_immediate(instr, addr);
	check_stack(pmach, pmach->_sp++, addr); // on incrémente sp et verifie qu'on ne sort pas de la pile
	ad_Data = get_adress(pmach, instr);
	check_overflow(pmach, ad_Data, addr);
	check_overflow(pmach, pmach->_sp, addr);
	pmach->_data[ad_Data] = pmach->_data[pmach->_sp]; // Data[Addr] <- Data[SP]

	return true;
}

/*\
 * \fn bool decode_execute(Machine *pmach, Instruction instr)
 * \brief Décodage et exécution d'une instruction
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction à exécuter
 * \return faux après l'exécution de \c HALT ; vrai sinon
 */bool decode_execute(Machine *pmach, Instruction instr) {
	unsigned addr = pmach->_pc - 1; //! adresse de l'instruction qu'on lit
	switch (instr.instr_immediate._cop) {
	case NOP:
		return true;
	case ILLOP:
		error(ERR_ILLEGAL, addr);
		return false;
	case LOAD:
		return modify_register(pmach, instr, addr, 0);
	case ADD:
		return modify_register(pmach, instr, addr, 1);
	case SUB:
		return modify_register(pmach, instr, addr, -1);
	case STORE:
		return store(pmach, instr, addr);
	case BRANCH:
	case CALL:
		return call_branch(pmach, instr, addr, instr.instr_immediate._cop);
	case RET:
		return ret(pmach, instr, addr);
	case PUSH:
		return push(pmach, instr, addr);
	case POP:
		return pop(pmach, instr, addr);
	case HALT:
		return false;
	default:
		error(ERR_UNKNOWN, addr);
		return true;
	}
}

//! Trace de l'exécution
/*!
 * \fn void trace(const char *msg, Machine *pmach, Instruction instr, unsigned addr)
 * \brief On écrit l'adresse et l'instruction sous forme lisible.
 * \param msg le message de trace
 * \param pmach la machine en cours d'exécution
 * \param instr l'instruction à exécuter
 * \param addr son adresse
 */void trace(const char *msg, Machine *pmach, Instruction instr, unsigned addr) {
	printf("TRACE: %s: 0x%04x: ", msg, addr);
	print_instruction(instr, addr);
	printf("\n");
}
