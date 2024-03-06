#  The simplest interrupt-driven program:
#  When Push button Key 1 is pressed
#  Toggle LED 3.  

.text
.global _start 
_start:
      movia sp, 0x20000    #  initialize the stack pointer (used in above interrupt service routine)
      
	  movia r2, KEYs		#  address of key pushbuttons in r2
	  
      movi r4, 0x1		    #  need to affect bit 0 using r4 of several registers!
	  
      stwio r4, 0xC(r2) 	#  this clears the edge capture bit for KEY0 if it was on, writing into the edge capture register
	  
	  stwio r4, 8(r2)		#  turn on the interrupt mask register bit 0 for KEY 0 so that this causes
	  					    #  an interrupt from the KEYs to go to the processor when button released
							
      movi r5, 0x2			#  need to turn on bit 1 below
	  
      wrctl ctl3, r5 		#  ctl3 also called ienable reg - bit 1 enables interupts for IRQ1->buttons
	  
      wrctl ctl0, r4 		#  ctl 0 also called status reg - bit 0 is Proc Interrupt Enable (PIE) bit#  
	  					    #  bit 1 is the User/Supervisor bit = 0 means supervisor
 
     
loop:				#  this is a do-nothing loop, except can keep an eye on r2 & r4
      movi r2,1		#  to make sure they aren't over-written
      movi r4,2
      br loop

      

	  

/* The code below is placed at the special address where all exceptions
   go to be handled, which is address 0x20.  The assembler knows to do this
   because of the .section .exceptions "ax" assembler directive.
*/
		.equ LEDs, 0xff200000
		.equ KEYs, 0xff200050
		
.section .exceptions, "ax"

/*  When the interrupt happens an we arrive here, several things are done automatically by the hardware:
		1) register r29 ea <- pc at instruction when interrupt happens
		2) register ctl1 <- ctl0  (i.e. a copy is made)   (could also say estatus <- status)
		3) ctl0 (status) bit 0 (PIE) <- 0, to disable all interrupts
		4) ctl0 (status) bit 1  (User/supervisor) <- 0 - put into supervisor mode
*/ 
      subi sp, sp, 8 #  save any registers the INT handler touches#  below uses r2 and r4, so need 8 bytes
      stw r2, 0(sp)  #  make sure you understand that how this pushes r2 and r4 onto the stack
      stw r4, 4(sp)

      movia r2, LEDs    #  address of LEDS
      ldwio r4, 0(r2) 	#  can also load the current state of LED register!
      xori r4, r4, 0x8  #  flip bit 3
      stwio r4, 0(r2)	#  store the changed bits back into the LED register
      
      movia r2, KEYs 
      movi r4, 1
      stwio r4, 12(r2) #  clear the edge bit that caused the int
      
      ldw r4, 4(sp) #  restore those two registers from stack by popping:
      ldw r2, 0(sp)
      addi sp, sp, 8
      
      subi ea, ea, 4  #   Need to go back an re-execute the interrupted instruction - move back
	  				  #   1 instruction (4 bytes) from where the pc was  
					  
					  
      eret			  #  does two things: 1) pc <- ea. (the instruction address to return to