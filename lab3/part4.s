.text

# Modify your program for Part III to do the following: 
# it should display the low-order 10 bits of the two resulting numbers (LargestOnes and LargesZeroes) on the LEDs, 
# in an infinite loop, one after the other.

.global _start

_start:
    movia r8, TEST_NUM   # begin at the start index of of TEST_NUM
    movi r5, 0 # r5 will keep track of the current largest ones
    movi r6, 0 # r6 will keep track of the current largest zeros
    movia sp, 0x20000 # sp = stack poitner 
    movi r19, 0xFFFFFFFF # for bit inversion 

    findLargest:
        ldw r4, 0(r8) # load the current number (index we are currently on in TEST_NUM) into r4
        beq r4, r0, finished # if the last word in TEST_NUM label is 0, then we have reached the end of the list
        # branch to finished if r4 = 0
        call ONES # run the subroutine to count the 1s
        bgt r10, r5, updateOnes # if the count in r10 is larger than r5, then update r5
        br continue

        continue:
            xor r4, r4, r19 # flip all the digits 
            movi r17, 1
            call ONES # run the subroutine to count the 0s
            bgt r10, r6, updateZeros
            br continue2
        continue2:
            addi r8, r8, 4 # increment the address pointer in TEST_NUM
            br findLargest

        updateOnes: # if we have found a new number with more set bits (more 1s)
            mov r5, r10 # update the current largest number 
            br continue # continue
        updateZeros:
            mov r6, r10
            br continue2
            
        
    finished:
        movia r11, LargestOnes     # address of the LargestOnes label
        stw r5, 0(r11)         # store the result of the number with the most set bits into r5
        movia r7, LargestZeroes
        stw r6, 0(r7) # store the result of the number with the most non set bits into r6

        .equ    LEDs, 0xFF200000
        movia r18, LEDs

    displayLoop:
        stwio r5, 0(r18)      # display r5 on the LEDs
        call Delay             

        stwio r6, 0(r18)      # display r6 on the LEDs
        call Delay            

        br displayLoop         # infinite loop it

    Delay:
        movi r9, 1500       

        outerLoop:
            movi r10, 10000     

            innerLoop:
                subi r10, r10, 1    
                bne r10, r0, innerLoop  

                subi r9, r9, 1     
                bne r9, r0, outerLoop   

                ret                 



    endiloop: br endiloop  # infinite loop to end the program

ONES:
    # saving the registers on the stack
    subi sp, sp, 12 # we want to save 4*2 = 8 
    stw ra, 0(sp)     
    # stw r10, 4(sp)    
    stw r16, 4(sp)    
    stw r17, 8(sp)   
    # stw r12, 12(sp) # loop counter - do we need to save this on the stack?

    movi r10, 0             # initialize the count of set bits to 0
    movi r12, 0            # initialize loop counter to 0
    movi r16, 32           # maximum number of bits
    movi r17, 1            # stores the number 1

    forLoop: 
        sll r13, r17, r12     # shift left the value 1 by r12 positions
        and r14, r4, r13       # bitwise AND r4 with r13
        beq r14, r0, iplusplus # if r14 is zero, skip the next line 
        addi r10, r10, 1         # increment the count of set bits

    iplusplus:
        addi r12, r12, 1       # increment the bit position counter
        blt r12, r16, forLoop  # if r12 is less than 32, continue loop
       

    # now restore the registers from the stack (pop) 
    ldw ra, 0(sp)     
    # ldw r10, 4(sp)    
    ldw r16, 4(sp)    
    ldw r17, 8(sp)  
    addi sp, sp, 12

    ret
    

.data
TEST_NUM:  .word 0x4a01fead, 0xF677D671,0xDC9758D5,0xEBBD45D2,0x8059519D
            .word 0x76D8F0D2, 0xB98C9BB5, 0xD7EC3A9E, 0xD9BADC01, 0x89B377CD
            .word 0  # end of list 

LargestOnes: .word 0
LargestZeroes: .word 0

