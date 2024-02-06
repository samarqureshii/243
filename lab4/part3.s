# same thing as part 2 except with the timer registers
.global _start
_start:
    .equ LEDs, 0xff200000
    .equ KEY_BASE, 0xff200050
    .equ TIMER_BASE, 0xff202000
    .equ COUNTER_START, 25000000
    
    movi r6, 1
    movi r7, 0 # bit mask for each KEY
   
    movi r10, 0 # toggle flag
    movi r13, 0 # ret value
    movi r14, 0 # global counter

    movia r18, KEY_BASE
    movia r19, LEDs 
    movia r20, TIMER_BASE

    # start the timer 
    stwio r0, (r20)
    movia r21, COUNTER_START
    srli r22, r21, 0xf # mask the upper 16 bits
    andi r21, r21, 0xffff # mask the lower 16 bits 
    stwio r21, 0x8(r20) # write to the timer period register (low)
    stwio r22, 0xc(r20) # write to the timer period register (high)
    movi r21, 0b110 # enable continuous mode and start the timer (START AND CONTINUE)
    stwio r21, 0x4(r20) # write to timer control register TO

    poll: 
        ldwio r12, 0xc(r18) # load EC reg
            
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


    delayCounter:
        ldwio r21, (r20)
        andi r21, r21, 1 # check the TO bit enabled or not
        beq r21, r0, delayCounter # keep waiting 

        stwio r0, (r20) # reset TO

        # ldwio r14, (r19)
        andi r14, r14, 0xFF 
        
        movi r15, 0xFF # cap it at 255
        beq r14, r15, resetCounter
        addi r14, r14, 1 # if not at 255, then increment 

        stwio r14, 0(r19) # update LEDs
        br poll
    
        resetCounter:
            movi r14, 0
            stwio r14, 0(r19) # update LEDs
            br poll 

ioCheck: # check if a key has been pressed and handle it
    and r13, r12, r7 # where r7 is the bit mask for every case
    beq r13, r0, notPressed # return 0

    # ;if a button was pressed, toggle local counter and reset EC bit
    xori r10, r10, 1
    stwio r7, 0xc(r18) # reset EC register
    movi r13, 1 # return 1
    ret

    notPressed:
        movi r13, 0
        ret