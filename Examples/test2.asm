// Copie d'éléments d'un endroit à un autre

        TEXT    30
        EQU     *
        LOAD    R01, #0
        LOAD    R02, #4

        LOAD    R03, +0[R01]
        STORE   R03, +0[R02]

        LOAD    R03, +1[R01]
        STORE   R03, +1[R02]

        LOAD    R03, +2[R01]
        STORE   R03, +2[R02]

        LOAD    R03, +3[R01]
        STORE   R03, +3[R02]

        HALT
        END

        DATA    30
        elem1    WORD    1
        elem2    WORD    2
        elem3    WORD    3
        elem4    WORD    4

        END
        

