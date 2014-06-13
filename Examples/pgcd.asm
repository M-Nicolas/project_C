//-----------------
// Instructions
//-----------------
			TEXT    30

main		EQU *
			PUSH @a
			PUSH @b
			CALL NC, @pgcd
			ADD R15, #2
			STORE R01, @result
			HALT
			NOP
			NOP
			NOP
			NOP
			// Sous-programme
pgcd		LOAD R00, 3[R15] // R00 = a
			LOAD R01, 2[R15] // R01 = b
			SUB R00, 2[R15] // a = a - b
			BRANCH GT, @pos
			BRANCH LT, @neg
			BRANCH EQ, @return

pos		STORE R00, 3[R15]
			BRANCH NC, @pgcd

neg		LOAD R02, 2[R15] // R02 = b
			SUB R02, 3[R15] // b = b - a
			STORE R02, 2[R15]
			BRANCH NC, @pgcd

return	RET

			END
//-----------------
// Données et pile
//-----------------
			DATA 10

			WORD 0
result	    WORD 0
a			WORD 96
b			WORD 36

			END
