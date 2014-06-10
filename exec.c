/*!
 * \file exec.c
 * \brief Exécution d'une instruction.
 */

#include "exec.h"
#include "error.h"

void check_stack(Machine *pmach, unsigned ad_Data, unsigned ad_Instr) {
	if (ad_Data < pmach->_dataend)
		error(ERR_SEGSTACK, ad_Instr);
}
//! Vérifie qu'il n'y a pas d'overflow dans le registre de données
/*!
 * \param pmach la machine/programme en cours d'exécution
 * \param ad_Instr l'adresse de l'instruction en cours d'execution
 * \param ad_Data l'adresse de la donnée dont on a besoin
 */
void check_overflow(Machine *pmach, unsigned ad_Data, unsigned ad_Instr) {
	if (ad_Data > pmach->_datasize)
		error(ERR_SEGDATA, ad_Instr);
}

//! Vérifie que l'instruction n'est pas codée en immédiate
/*!
 * \param instr l'instruction à exécuter
 * \param addr adresse de l'instruction
 * renvoie une erreur si l'instruction est codée en immédiate
 */void check_not_immediate(Instruction instr, unsigned addr) {
	if (instr.instr_generic._immediate) {
		error(ERR_IMMEDIATE, addr);
	}
}

//! Décodage et exécution de l'instruction LOAD
/*!
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction à exécuter
 * \param addr adresse de l'instruction
 * \return true
 */bool load(Machine *pmach, Instruction instr, unsigned addr) {
	unsigned ad_Data;
	if (instr.instr_generic._immediate) {
		pmach->_registers[instr.instr_generic._regcond] =
				instr.instr_immediate._value;
	} else {
		ad_Data = instr.instr_absolute._address;
		check_overflow(pmach, ad_Data, addr);
		printf("OK\n");
		pmach->_registers[instr.instr_generic._regcond] =
				pmach->_data[ad_Data];
	}

	return true;
}

//! Décodage et exécution de l'instruction STORE
/*!
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction à exécuter
 * \param addr adresse de l'instruction
 * \return true
 */bool store(Machine *pmach, Instruction instr, unsigned addr) {
	unsigned ad_Data;
	check_not_immediate(instr, addr);
	ad_Data = instr.instr_absolute._address;
	check_overflow(pmach, ad_Data, addr);
	pmach->_data[ad_Data] = pmach->_registers[instr.instr_generic._regcond];
	return true;
}

//! Décodage et exécution de l'instruction ADD
/*!
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction à exécuter
 * \param addr adresse de l'instruction
 * \return true
 */bool add(Machine *pmach, Instruction instr, unsigned addr) {
	unsigned ad_Data;
	if (instr.instr_generic._immediate) {
		pmach->_registers[instr.instr_generic._regcond] +=
				instr.instr_immediate._value;
	} else {
		ad_Data = instr.instr_absolute._address;
		check_overflow(pmach, ad_Data, addr);
		pmach->_registers[instr.instr_generic._regcond] +=
				pmach->_data[ad_Data];
	}

	return true;
}

//! Décodage et exécution de l'instruction SUB
/*!
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction à exécuter
 * \param addr adresse de l'instruction
 * \return true
 */bool sub(Machine *pmach, Instruction instr, unsigned addr) {
	unsigned ad_Data;
	if (instr.instr_generic._immediate) {
		pmach->_registers[instr.instr_generic._regcond] -=
				instr.instr_immediate._value;
	} else {
		ad_Data = instr.instr_absolute._address;
		check_overflow(pmach, ad_Data, addr);
		pmach->_registers[instr.instr_generic._regcond] -=
				pmach->_data[ad_Data];
	}

	return true;
}

//! Affecte pour chaque condition la valeur que doit contenir le code condition de la machine
/*!
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

//! Décodage et exécution de l'instruction BRANCH
/*!
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction à exécuter
 * \param addr adresse de l'instruction
 * \return true
 */bool branch(Machine *pmach, Instruction instr, unsigned addr) {
	unsigned ad_Data;
	check_not_immediate(instr, addr);
	if (check_condition(pmach, instr, addr)) {
		ad_Data = instr.instr_absolute._address;
		pmach->_pc = ad_Data;
	}
	return true;
}

//! Décodage et exécution de l'instruction CALL
/*!
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction à exécuter
 * \param addr adresse de l'instruction
 * \return true
 */bool call(Machine *pmach, Instruction instr, unsigned addr) {
	check_not_immediate(instr, addr);
	if (check_condition(pmach, instr, addr)) {
		check_overflow(pmach, pmach->_sp, addr);
		pmach->_data[pmach->_sp] = pmach->_pc;
		check_stack(pmach, pmach->_sp--, addr);
		pmach->_sp--;
		pmach->_pc = instr.instr_absolute._address;
	}
	return true;
}

//! Décodage et exécution de l'instruction RET
/*!
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction à exécuter
 * \param addr adresse de l'instruction
 * \return true
 */bool ret(Machine *pmach, Instruction instr, unsigned addr) {
	check_overflow(pmach, pmach->_sp++, addr);
	pmach->_sp++;
	pmach->_pc = pmach->_data[pmach->_sp];
	return true;
}

//! Décodage et exécution de l'instruction PUSH
/*!
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction à exécuter
 * \param addr adresse de l'instruction
 * \return true
 */bool push(Machine *pmach, Instruction instr, unsigned addr) {
	unsigned ad_Data;
	if (instr.instr_generic._immediate) {
		check_overflow(pmach, pmach->_sp, addr);
		pmach->_data[pmach->_sp] = instr.instr_immediate._value;
	} else {
		check_overflow(pmach, pmach->_sp, addr);
		ad_Data = instr.instr_absolute._address;
		check_overflow(pmach, ad_Data, addr);
		pmach->_data[pmach->_sp] = pmach->_data[ad_Data];
	}
	check_stack(pmach, pmach->_sp--, addr);
	pmach->_sp--;
	return true;
}

//! Décodage et exécution de l'instruction POP
/*!
 * \param pmach la machine/programme en cours d'exécution
 * \param instr l'instruction à exécuter
 * \param addr adresse de l'instruction
 * \return true
 */bool pop(Machine *pmach, Instruction instr, unsigned addr) {
	unsigned ad_Data;

	check_not_immediate(instr, addr);
	check_stack(pmach, pmach->_sp--, addr);
	pmach->_sp--;
	ad_Data = instr.instr_absolute._address;
	check_overflow(pmach, ad_Data, addr);
	check_overflow(pmach, pmach->_sp, addr);
	pmach->_data[ad_Data] = pmach->_data[pmach->_sp];

	return true;
}

//! Décodage et exécution d'une instruction
/*!
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
		return load(pmach, instr, addr);
	case STORE:
		return store(pmach, instr, addr);
	case ADD:
		return add(pmach, instr, addr);
	case SUB:
		return sub(pmach, instr, addr);
	case BRANCH:
		return branch(pmach, instr, addr);
	case CALL:
		return call(pmach, instr, addr);
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
 * On écrit l'adresse et l'instruction sous forme lisible.
 *
 * \param msg le message de trace
 * \param pmach la machine en cours d'exécution
 * \param instr l'instruction à exécuter
 * \param addr son adresse
 */
void trace(const char *msg, Machine *pmach, Instruction instr, unsigned addr) {
	printf("TRACE: %s: 0x%04x: ", msg, addr);
	print_instruction(instr,addr);
	printf("\n");
}
