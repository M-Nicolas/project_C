//-----------------
// Instructions
//-----------------
        TEXT 30

        // Programme principal
main    EQU *
        LOAD R00, @aa
        LOAD R01, @bb
        LOAD R02, +0[R00]
	LOAD R03, +0[R01]
        BRANCH NC, @pgcd
        HALT 
        NOP 
        NOP 
        NOP 
        NOP

        // Sous-programme
pgcd	EQU *
        SUB R02, 0[R03]
	BRANCH GT, @sous
        BRANCH LT, @sous2
sous    SUB R00, 0[R01]
sous2	SUB R01, 0[R00]
        STORE R00, 0[R08]
	STORE R01, 0[R09]
	LOAD R02, 0[R08]
	LOAD R03, 0[R09]
	SUB R02, 0[R03]
	BRANCH NE, @pgcd
        BRANCH EQ, @return
return  RET
        
        END
        
//-----------------
// Données et pile
//-----------------
        DATA 30
        
aa	WORD 0x06
bb      WORD 0x03
        
        END
