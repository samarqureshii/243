# enable two types of interrupts, Timer and KEY push buttons
# call CONFIG_TIMER and CONFIG_KEYS subroutines 
# CONFIG_TIMER generates one interupt every 0.25 seconds 

# 1. main program executes endless loop, writing the value of the global variable COUNT to the red lights LEDR
# 2. write ISR for timer and keys
# 3. modify IRQ_HANDLER so that it calls correct ISR
# 4. in timer ISR, increment the global COUNT variable by the value of the RUN global variable, which is either 0 or 1
    # TO bit in status register is set to 1 when timer reaches a count of 0
    # possible to generate an interrupt when this occurs, by using the ITO bit in the Control register 
    # setting the ITO bit to 1 causes an interrupt request to be sent to the processor whenever TO becomes 1
    # after an interrupt occurs, it can be cleared by writing any value into the Status register 
# 5. toggle the value of the RUN global variable in the ISR for the KEYs, each time a KEY is pressed.
    # when RUN = 0, main program will display static count on LEDs 
    # when RUN = 1, the count shown on the LEDs (in binary) will increment every 0.25s


.section .exceptions, "ax"
/******************************************************************************
 * Write an interrupt service routine
 *****************************************************************************/
.section .exceptions, "ax"
IRQ_HANDLER: # save context registers, determine source fo input, call appropriate ISR, restore context
        # save registers on the stack (et, ra, ea, others as needed)
        subi    sp, sp, 16          # make room on the stack
        stw     et, 0(sp)
        stw     ra, 4(sp)
        stw     r20, 8(sp)

        rdctl   et, ctl4            # read exception type
        beq     et, r0, SKIP_EA_DEC # not external?
        subi    ea, ea, 4           # decrement ea by 4 for external interrupts

SKIP_EA_DEC:
        stw     ea, 12(sp)
        andi    r20, et, 0b10  
        beq     r20, r0, HANDLE_TIMER
        call KEY_ISR       

HANDLE_TIMER:     
        call TIMER_ISR                              

END_ISR:
        ldw     et, 0(sp)           # restore registers
        ldw     ra, 4(sp)
        ldw     r20, 8(sp)
        ldw     ea, 12(sp)
        addi    sp, sp, 16          # restore stack pointer
        eret                        # return from exception

/*********************************************************************************
 * set where to go upon reset
 ********************************************************************************/
.section .reset, "ax"
        movia   r8, _start
        jmp    r8

.text
.global  _start

.equ LED_BASE, 0xff200000
.equ KEY_BASE, 0xff200050 
.equ TIMER_BASE, 0xff202000
.equ COUNTER_START, 7000000

_start:
    /* Set up stack pointer */
    movia sp, 0x20000
    call    CONFIG_TIMER        # configure the Timer
    call    CONFIG_KEYS         # configure the KEYs port
    movi r2, 0b10

    movi r23, 0b11
    wrctl ctl3, r23 # bit 0 enables interupts for IRQ0 -> timer, bit 1 enables interupts for IRQ1->button

    movi r24, 0b1
    wrctl ctl0, r24 # ; ctl 0 also called status reg - bit 0 is Proc Interrupt Enable (PIE) bit, 
    # bit 1 is the User/Supervisor bit = 0 means supervisor

    movia   r8, LED_BASE        # LEDR base address (0xFF200000)  
    movia r9, COUNT 

LOOP:
    ldw     r10, 0(r9)          # global variable
    stwio   r10, 0(r8)          # write to the LEDR lights
    br      LOOP

# use r8 - r15 and are not saved 
CONFIG_TIMER: # generates one interrupt every 0.25s
    movia r11, COUNTER_START
    movia   r10, TIMER_BASE

    movi r9, 0x8 # stop timer in case it was running
    stwio r9, 0x4(r10)

    stwio r0, (r10) # reset timer 

    srli r12, r11, 0xf # mask the upper 16 bits
    andi r11, r11, 0xffff # mask the lower 16 bits 
    stwio r11, 0x8(r10) # write to the timer period register (low)
    stwio r12, 0xc(r10) # write to the timer period register (high)

    movi r11, 0b111 # enable continuous mode and start the timer and interrupt (START AND CONTINUE and ITO bits)
    stwio r11, 0x4(r10) # write to timer control register TO 

    /*ldw r22, 4(sp)
    ldw r21, 0(sp)
    addi sp, sp, 4*/

    ret

# use r8 - r15
CONFIG_KEYS: 
    movia   r8, KEY_BASE  
    movi r9, 0b1111
    stwio r9, 0xc(r8) # clear edge capture
    stwio r9, 0x8(r8) # turn on the interrupt mask register for all KEYs
    
    ret

# use r16 - r23 and use the stack to save 
KEY_ISR: # INTERRUPT SERVICE ROUTINE FOR IF A KEY WAS PRESSED
    # IN KEY_ISR:
    # When KEY0 is pressed, RUN variable should be toggled (start/stop the timer)
    # When KEY1 is pressed, rate at which COUNT is incremented is doubled (shift to the left)
    # When KEY2 pressed, rate should be halved (shift to the  right)
    # To change the rate, stop the timer within the KEY service routine, modify value in the load timer, and restart the timer 
    subi sp, sp, 36
    stw r16, 0(sp)
    stw r17, 4(sp)
    stw r18, 8(sp)
    stw r19, 12(sp)
    stw r20, 16(sp)
    stw r21, 20(sp)
    stw r22, 24(sp)
    stw r23, 28(sp)
    stw r24, 32(sp)

    movia r16, KEY_BASE
    ldwio r17, 0xc(r16)  


    andi r18, r17, 0x1       
    beq r
    andi r19, r17, 0x2       
    andi r20, r17, 0x4       

    # clear the Edge Capture Register to acknowledge the interrupt
    movi r21, 0b1111
    stwio r21, 0xc(r16)
    
    bne r18, r0, TOGGLE_RUN
    bne r19, r0, DOUBLE_RATE
    bne r20, r0, HALVE_RATE

    br EXIT_ISR

    TOGGLE_RUN:
        movia r22, RUN
        ldw r23, 0(r22)
        xori r23, r23, 0x1
        stw r23, 0(r22)
        br EXIT_ISR

    DOUBLE_RATE:
        movia r16, TIMER_BASE
        movia r22, COUNTER_START
        ldw r23, (r22)
        # movi r24, 2147483647
        # bge r23, r24, SKIP_DOUBLE
        slli r23, r23, 1    # double the rate
        # SKIP_DOUBLE:
            stw r23, (r22) # store the new doubled rate into COUNTER_START global var 
            call CONFIG_TIMER
        /*stwio r0, 4(r16) # stop the timer
        stwio r0, 0(r16) # reset the timer 

        # load the new timer period 
        srli r24, r23, 16
        andi r23, r23, 0xffff
        stwio r23, 0x8(r16)
        stwio r24, 0xc(r16)
        
        # restart the timer with new configurations
        movi r17, 0b111 # enable cont, start, and enable interrupt
        stwio r17, 0x4(r16)*/
        br EXIT_ISR

    HALVE_RATE:
        movia r16, TIMER_BASE
        movia r22, COUNTER_START
        ldw r23, 0(r22)
        srli r23, r23, 1     # half the rate 
        stw r23, 0(r22) # store the new halved rate into COUNTER_START global var
        call CONFIG_TIMER
        /*, r23, 0xffff
        stwio r23, 0x8(r16)
        stwio r24, 0xc(r16)
        
        # restart the timer with new configurations
        movi r17, 0b111 # enable cont, start, and enable interrupt
        stwio r17, 0x4(r16)*/
        br EXIT_ISR

    EXIT_ISR:
        # clear Edge Capture register to prepare for the next interrupt 
        movi r17, 0b1111 # could also write 0b1111 to make sure we cleared ALL the keys 
        stwio r17, 0xc(r16) # clear edge capture register so we can prepare to handle the next interrupt

        ldw r24, 32(sp)
        ldw r23, 28(sp)
        ldw r22, 24(sp)
        ldw r21, 20(sp)
        ldw r20, 16(sp)
        ldw r19, 12(sp)
        ldw r18, 8(sp)
        ldw r17, 4(sp)
        ldw r16, 0(sp)
        addi sp, sp, 36
        ret

# use r16 - r23 and use the stack to save 
TIMER_ISR: # INTERRUPT SERVICE ROUTINE FOR TIMER 
    subi sp, sp, 20
    stw r16, 0(sp)
    stw r17, 4(sp)
    stw r18, 8(sp)
    stw r19, 12(sp)
    stw r20, 16(sp)

    movia r16, COUNT
    movia r17, RUN

    movia r18, TIMER_BASE
    stwio r0, (r18) # clear timer interrupt flag by writing any value to timer status register 

    ldw r19, (r16) # load the count value
    ldw r20, (r17) # load the run value 
    add r19, r19, r20 # add RUN to COUNT
    # now store new COUNT value back into memory
    stw r19, (r16)

    ldw r20, 16(sp)
    ldw r19, 12(sp)
    ldw r18, 8(sp)
    ldw r17, 4(sp)
    ldw r16, 0(sp)
    addi sp, sp, 20

    ret


.data
/* Global variables */
.global  COUNT
COUNT:  .word    0x0            # used by timer

.global  RUN                    # used by pushbutton KEYs
RUN:    .word    0x1            # initial value to increment COUNT

.end

