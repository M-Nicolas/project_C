// Test de dépiler quand la pile est vide

        TEXT 20
        EQU     *
        LOAD    R00, #10
        LOAD    R04, #9
        STORE    R04, @0000
        STORE    R00, @0001
	POP @0000
        HALT
        END

        DATA 30
        WORD    0
        END
        
