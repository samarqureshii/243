/* Program to Count the number of 1’s in a 32-bit word,
located at InputWord */
.global _start
_start:
call ones
endiloop: br endiloop

ones: 

/* Your code here */
    movia r3, InputWord
    movia r7, Answer
    
    ldw r5, (r3)
    
    movi r2, 0   # Initialize r2 to 0 for counting    
    loop:
        beq r5, r0, done
        andi r4, r5, 1
        srli r5, r5, 1 #shift by 1
        bne r4, r0, land
        br loop
    land:
        addi r2, r2, 1
        br loop
    done:
    stw r2, (r7)
    
ret


.data
InputWord: .word 0x4a01fead
Answer: .word 0