.text  # The numbers that turn into executable instructions
.global _start
_start:

/* r13 should contain the grade of the person with the student number, -1 if not found */
/* r10 has the student number being searched */

    movia r10, 718293        # r10 is where you put the student number being searched for

/* Your code goes here  */
    movia r17, result
    
    movia r11, Snumbers #Put Snumbers into r11
    movia r14, Grades #put grades into r14
    
    
loop:
    addi r13, r13, 1 # Increment counter
    ldw r12, (r11) # Put first number into r12
    beq    r12, r0, gloop # exit loop when Snumbers is at end of array
    addi r11, r11, 4 # Advance 1 word address through Snumbers
    bne r10, r12, loop # loop if not equal to search value
    br gloop
    
gloop:
    ldw r15, (r14)
    subi r13, r13, 1
    ble r13, r0, found
    
    addi r14, r14, 4
    br gloop
    
found:
    stw r15, (r17)
	
	
.equ LEDs, 0xFF200000
movia r18, LEDs
stwio r15, (r18)

iloop: br iloop


.data      # the numbers that are the data 

# .skip 3

/* result should hold the grade of the student number put into r10, or
-1 if the student number isn't found */ 

result: .word -1
        
/* Snumbers is the "array," terminated by a zero of the student numbers  */
Snumbers: .word 10392584, 423195, 644370, 496059, 296800
        .word 265133, 68943, 718293, 315950, 785519
        .word 982966, 345018, 220809, 369328, 935042
        .word 467872, 887795, 681936, 0

/* Grades is the corresponding "array" with the grades, in the same order*/
Grades: .word 99, 68, 90, 85, 91, 67, 80
        .word 66, 95, 91, 91, 99, 76, 68  
        .word 69, 93, 90, 72
		
