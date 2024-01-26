/* Program to Count the number of 1's in a 32-bit word,
located at InputWord */

# You are to write a Nios II assembly language program that will count the number of ones in the binary number 
# representation of a given input 32-bit word. 
# For example, if the word had the value 0x4a01fead (hex/base 16) then the result would be 16 (in base 10). 
# The “input” to your code will be set with an assembler directive (.word) as shown in the skeleton code given in Figure 1, 
# where the assembler directive sets word at address InputWord to be have the value 0x4a01fead. 
# Your program must work correctly for any value placed there.
#  The result (the number of 1’s in the input), must be stored into the word Answer

.global _start

.section .data
InputWord: .word 0x4a01fead
Answer: .word 0

.section .text
_start:
    movia r8, InputWord    # address of the input word
    ldw r9, 0(r8)          # load the input word into r9
    movi r10, 0            # r10 will count the number of set bits
    movia r11, Answer      # address of the Answer label
    movi r12, 0            # r12 will be the counter variable in the loop
    movi r16, 32           # maximum number of bits
    movi r17, 1            # stores the number 1

forLoop:
    sll r13, r17, r12     # shift left the value 1 by r12 positions
    and r14, r9, r13       # bitwise AND r9 with r13
    bne r14, r0, setBit    # if r14 is not zero, then the bit is set
    br notSet

setBit:
    addi r10, r10, 1       # increment the count of set bits

notSet:
    addi r12, r12, 1       # increment the bit position counter
    blt r12, r16, forLoop  # if r12 is less than 32, continue loop

    stw r10, 0(r11)        # store the result into Answer
    br updateLeds

updateLeds:
    .equ    LEDs, 0xFF200000
    movia r18, LEDs
    stwio r10, 0(r18)      # display r10 on the LEDs

endiloop:
    br endiloop            # infinite loop to end the program
