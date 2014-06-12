// Copie d'un tableau dans un autre :

        TEXT    30
        EQU     *
        LOAD    R01, @13
        LOAD    R02, @14

        LOAD    R03, +4[R01]
        STORE   R03, +4[R02]
        LOAD    R03, +4[R01]
        STORE   R03, +4[R02]
        LOAD    R03, +4[R01]
        STORE   R03, +4[R02]

        LOAD    R03, +0[R01]
        STORE   R03, +0[R02]
        END

        DATA    30
        WORD    0

        END
        

