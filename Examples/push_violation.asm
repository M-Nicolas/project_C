// Test d'empiler quand la pile est pleine

        TEXT 6
        EQU     *
        LOAD    R00, #10
        LOAD    R04, #9
        STORE    R04, @0000
        STORE    R00, @0001
	PUSH @0000
	PUSH @0000
	PUSH @0000
	PUSH @0000
	PUSH @0000
	PUSH @0000
	PUSH @0000
	PUSH @0000
	PUSH @0000
	PUSH @0000
	PUSH @0000
	PUSH @0000
	PUSH @0000
	PUSH @0000
	PUSH @0000
	PUSH @0000
	PUSH @0000
	PUSH @0000
	PUSH @0000
	PUSH @0000
	PUSH @0000
        HALT
        END

        DATA 30
        WORD    1
        WORD    2
        WORD    3
        WORD    4
        WORD    5
        WORD    6
        WORD    7
        WORD    8
        WORD    9
        WORD    10
        WORD    11
        WORD    12
        WORD    13
        WORD    14
        WORD    15
        WORD    16
        WORD    17
        WORD    18
        END
        
