# write an ISR to display certain values on the HEX based on KEY interrupts


/******************************************************************************
 * Write an interrupt service routine
 *****************************************************************************/
.section .exceptions, "ax"

/*  when the interrupt happens an we arrive here, several things are done automatically by the hardware:
		1) register r29 ea <- pc at instruction when interrupt happens
		2) register ctl1 <- ctl0  (i.e. a copy is made)   (could also say estatus <- status)
		3) ctl0 (status) bit 0 (PIE) <- 0, to disable all interrupts
		4) ctl0 (status) bit 1  (User/supervisor) <- 0 - put into supervisor mode
*/ 

IRQ_HANDLER:
        # save any registers the INT handler touches
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
        andi    r20, et, 0x2        # check if interrupt is from pushbuttons
        beq     r20, r0, END_ISR    # if not, ignore this interrupt
        call    KEY_ISR             # if yes, call the pushbutton ISR

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

/*********************************************************************************
 * Main program
 ********************************************************************************/
.text
.global  _start

.equ KEY_BASE, 0xff200050 
.equ HEX_BASE1, 0xff200020
.equ HEX_BASE2, 0xff200030

_start:
        
        # 1. INIT SP
        movia sp, 0x20000 # sp = stack pointer 

        # 2. SET UP KEYS TO GENERATE INTERRUPTS
        # If KEY0 pressed, then display 0 on HEX0, if KEY1 pressed, then display 1 on HEX1, etc
        # ;if we press and release a key again, we blank the display if it was previously on 
        movi r1, 0x1 # need to turn on bit 0 in several registers
        movi r2, 0x2 # may need to turn on bit 1 elsewhere 
        movi r4, 0 # initialize r4 with 0. This register keeps track of 4 bit value that will be displayed on the HEX (max 0b1111 for "F")
        movi r5, 0 # register stores which HEX we will display the value on (max 0b101 for HEX5)
        movia r18, KEY_BASE
        
        stwio r1, 0xc(r18) # clear edge capture bit for KEY0 if it was on, writing into EC reg
        stwio r1, 0x8(r18) # turn on the interrupt mask register bit 0 for KEY 0 


        # 3. ENABLE INTERUPTS IN NIOS II
        wrctl ctl3, r2 # ctl3 also called ienable reg - bit 1 enables interupts for IRQ1->buttons
        wrctl ctl0, r1 # ; ctl 0 also called status reg - bit 0 is Proc Interrupt Enable (PIE) bit, 
        # bit 1 is the User/Supervisor bit = 0 means supervisor
        
IDLE:   br  IDLE

KEY_ISR:
        ldwio r19, 0xc(r18) # load Edge Capture reg for the KEY_BASE
        movia r20, HEX_BASE1
        movi r21, 0 # temp reg for each of the results of the bit masking 

        # check to see which KEY caused the interrupt (prof rose: was it you, was it you, was it you?)
        andi r21, r19, 0x1
        bne r21, r0, DISPLAY_HEX0
        andi r21, r19, 0x2
        bne r21, r0, DISPLAY_HEX1
        andi r21, r19, 0x4
        bne r21, r0, DISPLAY_HEX2
        andi r21, r19, 0x8
        bne r21, r0, DISPLAY_HEX3
        br END_ISR # if the interrupt was caused somewhere else?


        DISPLAY_HEX0:
                movi r4, 0
                movi r5, 0
                br cont
        DISPLAY_HEX1:
                movi r4, 1
                movi r5, 1
                br cont
        DISPLAY_HEX2:
                movi r4, 2
                movi r5, 2
                br cont
        DISPLAY_HEX3:
                movi r4, 3
                movi r5, 3
                br cont

        cont: # now update the HEX
                # ;push operation
                subi sp, sp, 8     
                stw r4, 4(sp)      
                stw r5, 0(sp)     

                call HEX_DISP  

                # ;pop operation
                ldw r4, 4(sp)      
                ldw r5, 0(sp)     
                addi sp, sp, 8    

                stwio r19, 0xc(r18) # clear edge capture register 
                br END_ISR


HEX_DISP:   movia    r8, BIT_CODES         # starting address of the bit codes
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
			
