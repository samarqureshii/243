.global _start
_start: ;annies code

    movi r8, 1 ⁠constant dont change
    movi r9, 1 #initial value
    movi r10, 30 #loop until
    movi r12, 1 

loop:
    add r9, r9, r8 #add 1 to r9 store in r9
    add r12, r12, r9 #add r9 to r9 store in r10
    blt r9, r10, loop

.equ LEDs, 0xFF200000
movia r13, LEDs
stwio r12, (r13)
done:
    br done