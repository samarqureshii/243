# take the part I code and make a subroutine called ONES
# have the subroutine use register r4 to receive input data to be processed
# use r2 to return the resulting count
# place the result into memory

.global _start

.section .data
InputWord: .word 0x4a01fea
Answer: .word 0

.section .text
_start:
    movia r8, InputWord    # address of the input word
    ldw r4, 0(r8)          # load the input word into r4

    call ONES              # call the subroutine 

    movia r11, Answer      # address of the Answer label
    stw r2, 0(r11)         # store the result from r2 into Answer

    .equ    LEDs, 0xFF200000
    movia r18, LEDs
    stwio r2, 0(r18)      # display r2 on the LEDs

    endiloop: br endiloop  # infinite loop to end the program

ONES:
    movi r2, 0             # initialize the count of set bits to 0
    movi r12, 0            # initialize loop counter to 0
    movi r16, 32           # maximum number of bits
    movi r17, 1            # stores the number 1

    forLoop: 
        sll r13, r17, r12     # shift left the value 1 by r12 positions
        and r14, r4, r13       # bitwise AND r4 with r13
        beq r14, r0, iplusplus # if r14 is zero, skip the next line 
        addi r2, r2, 1         # increment the count of set bits

    iplusplus:
        addi r12, r12, 1       # increment the bit position counter
        blt r12, r16, forLoop  # if r12 is less than 32, continue loop
    ret                        # return to caller after loop



