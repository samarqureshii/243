/* Program to Count the number of 1’s in a 32-bit word,
located at InputWord */
.global _start
_start:

    movia r10, TEST_NUM  # the address of the numbers is in r10
    movi r11, 0
    
    
    
    /* loop to search for the biggest number */

    oloop: 
        ldw r20, (r10)  # load the current TEST_NUM
        beq r20, r0, reset
		
		

        call ONES

        addi r10, r10, 4   # add 4 to the pointer to the numbers to point to the next one

        bge r11, r2, oloop  # if the current biggest is still biggest, go to floop

        mov r11, r2   # otherwise, the new number is the biggest, put it into r11
        br oloop
        
    reset:
        movia r10, TEST_NUM
        movi r2, 0
        movi r21, 0
        movia r3, 0xFFFFFFFF
    
    zloop: 
        ldw r20, (r10)
        beq r20, r0, endiloop
        xor r20, r20, r3
        
        call ONES
        
        addi r10, r10, 4   # add 4 to the pointer to the numbers to point to the next one

        bge r21, r2, zloop  # if the current biggest is still biggest, go to zloop
        mov r21, r2 
        # otherwise, the new number is the biggest, put it into r21
        br zloop

    endiloop:
        br endiloop

ONES: 
    /* Your code here */
    movi r2, 0   # Initialize r2 to 0 for counting
  
    movia r7, Answer

    mov r5, r20

    loop: 
        beq r5, r0, done
        andi r4, r5, 1
        srli r5, r5, 1  # shift by 1
        bne r4, r0, land
        br loop

    land: 
        addi r2, r2, 1
        br loop

    done: 
    ret

.data

Answer: .word 0

TEST_NUM: .word 0x4a01fead, 0xF677D671,0xDC9758D5,0xEBBD45D2,0x8059519D
        .word 0x76D8F0D2, 0xB98C9BB5, 0xD7EC3A9E, 0xD9BADC01, 0x89B377CD
        .word 0 # end of list
LargestOnes: .word 0
LargestZeroes: .word 0