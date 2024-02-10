# write assembly program that displays full functionality of HEX_DISP subroutine

# program should not use interrupts, and should be shortest, simplest demonstration that the subroutine works as advertised

/*    Subroutine to display a four-bit quantity as a hex digits (from 0 to F) 
      on one of the six HEX 7-segment displays on the DE1_SoC.
*
 *    Parameters: the low-order 4 bits of register r4 contain the digit to be displayed
		  if bit 4 of r4 is a one, then the display should be blanked
 *    		  the low order 3 bits of r5 say which HEX display number 0-5 to put the digit on
 *    Returns: r2 = bit patterm that is written to HEX display
 */	

.equ HEX_BASE1, 0xff200020
.equ HEX_BASE2, 0xff200030
.equ TIMER_BASE, 0xff202000
.equ COUNTER_START, 20000000


.global _start
_start:
    movi r5, 0      # Set r5 to 0 to select HEX0 for display
    movi r4, 0      # Start with the first digit to display
    movi r14, 0b10000
    movia sp, 0x20000 # sp = stack poitner 
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

display_loop:
movi r4, 0

digit_loop:
    # ;push operation
    subi sp, sp, 8     
    stw r4, 4(sp)      
    stw r5, 0(sp)     

    call HEX_DISP  

    # ;pop operation
    ldw r4, 4(sp)      
    ldw r5, 0(sp)     
    addi sp, sp, 8    

    delay:
        ldwio r21, (r20)
        andi r21, r21, 1 # check the TO bit enabled or not
        beq r21, r0, delay # keep waiting until the TO is enabled (delay counter has stopped, and global counter can now increment)
        stwio r0, (r20) # reset TO

    # increment the digit and wrap around from 0xF back to 0
    addi r4, r4, 1   # increment the digit
    # andi r4, r4, 0xF # ensure the digit wraps around from F to 0
    bne r4, r14, digit_loop

    # ;if not equal to 0, then we increment the HEX display and blank the current one 
    movi r4, 0b10000 # blank the current hex by writing 1 to the 4th bit
    
    subi sp, sp, 8     
    stw r4, 4(sp)      
    stw r5, 0(sp)     

    call HEX_DISP  

    # ;pop operation
    ldw r4, 4(sp)      
    ldw r5, 0(sp)     
    addi sp, sp, 8    

    addi r5, r5, 1 # increment the HEX display
    # andi r5, r5, 0b101 # ensure the HEX displays wrap around back to HEX0 when we finish HEX5

    # loop back to display the next digit
    br display_loop


HEX_DISP:   
        movia    r8, BIT_CODES         # starting address of the bit codes
	    andi     r6, r4, 0x10	   # get bit 4 of the input into r6
	    beq      r6, r0, not_blank 
	    mov      r2, r0
	    br       DO_DISP
not_blank:  andi     r4, r4, 0x0f	   # r4 is only 4-bit
            add      r4, r4, r8            # add the offset to the bit codes
            ldb      r2, 0(r4)             # index into the bit codes

# Display it on the target HEX display
DO_DISP:    
			movia    r8, HEX_BASE1         # load address
			movi     r6,  4
			blt      r5,r6, FIRST_SET      # hex4 and hex 5 are on 0xff200030
			sub      r5, r5, r6            # if hex4 or hex5, we need to adjust the shift
			addi     r8, r8, 0x0010        # we also need to adjust the address
FIRST_SET:
			slli     r5, r5, 3             # hex*8 shift is needed
			addi     r7, r0, 0xff          # create bit mask so other values are not corrupted
			sll      r7, r7, r5 
			addi     r4, r0, -1
			xor      r7, r7, r4  
    			sll      r4, r2, r5            # shift the hex code we want to write
			ldwio    r5, 0(r8)             # read current value       
			and      r5, r5, r7            # and it with the mask to clear the target hex
			or       r5, r5, r4	           # or with the hex code
			stwio    r5, 0(r8)		       # store back
END:			
			ret
			
BIT_CODES:  .byte     0b00111111, 0b00000110, 0b01011011, 0b01001111
			.byte     0b01100110, 0b01101101, 0b01111101, 0b00000111
			.byte     0b01111111, 0b01100111, 0b01110111, 0b01111100
			.byte     0b00111001, 0b01011110, 0b01111001, 0b01110001

            .end
			
