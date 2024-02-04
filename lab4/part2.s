.global _start
_start:
    .equ KEY_BASE, 0xff200050        
    .equ LEDs, 0xFF200000
    .equ COUNTER_DELAY, 500000 # 10000000 on De1

    movi r6, 1
    movi r7, 0 # bit mask for each KEY
    movia r8, KEY_BASE
    movi r10, 0 # toggle flag
    movi r13, 0 # ret value
    movi r14, 0 # global counter
    movia r9, LEDs 
    
    poll: 
        ldwio r12, 0xc(r8) # load EC reg
            
        # check each of the keys, and if pressed, toggle the counter running flag
        movi r7, 0x1 # KEY0
        call ioCheck
            
        movi r7, 0x2 # KEY1
        call ioCheck

        movi r7, 0x4 # KEY2
        call ioCheck

        movi r7, 0x8 # KEY3
        call ioCheck

        beq r10, r0, poll # if the counter is idle, keep polling

        # ;if not, enter delay loop
        movia r11, COUNTER_DELAY
    delayCounter:
        subi r11, r11, 1
        bne r11, r0, delayCounter

        addi r14, r14, 1
        andi r14, r14, 0xFF 
        stwio r14, 0(r9)
        
        movi r15, 0xFF
        beq r14, r15, resetCounter
        br poll
        
        resetCounter:
            movi r14, 0
            stwio r14, 0(r9) 
            br poll 

ioCheck: # check if a key has been pressed and handle it
    and r13, r12, r7 # where r7 is the bit mask for every case
    beq r13, r0, notPressed # return 0

    # ;if a button was pressed, toggle local counter and reset EC bit
    xori r10, r10, 1
    stwio r7, 0xc(r8) # reset EC register
    movi r13, 1 # return 1
    ret

    notPressed:
        movi r13, 0 return 0
        ret
