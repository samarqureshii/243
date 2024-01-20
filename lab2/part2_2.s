# Make a new version of your code in which the Grades are stored as 8-bit bytes, rather than full words. 
# That is, change the .word assembler directives for the Grades list to be .byte, as well as the directive for the result.

.text  # The numbers that turn into executable instructions
.global _start
_start:

/* r13 should contain the grade of the person with the student number, -1 if not found */
/* r10 has the student number being searched */


movia r10, SnumberToSearch # r10 is where you put the address of the student number being searched for

# use r11 to iterate through each of the addresses in the Snumbers label 
movia r11, Snumbers # begin at the start index of SNumbers

# use r12 to store the current student number when we iterate through the list

# store the student number from address in r10 in r13
ldb r13, (r10)

# use r14 as the pointer iterator for the Grades
movia r14, Grades

# use r15 to load the address of the result label
movia r15, result  

# figure out the grade based on the student number- determine the number of addresses in memory we need to iterate through
# to reach the student number 

loop:   
        # load the current student number into r12
        ldb r12, (r11)

        beq r12, r0, notFound # jump to the notFound label if we iterate through all the student numbers and don't find the one we are looking for 
        beq r13, r12, found # jump to the found label if we have matched the student number with the one we want 

        # increment the address pointers 
        addi r11, r11, 4 # Snumber index 
        addi r14, r14, 1 # Grades index, we iterate by 1 instead bc its bytes instead of byte addressable words 

        br loop

found: 
        ldb r13, (r14) # load the grade into r13
        stw r13, (r15) # now store it into result 
        br iloop

notFound: 
        movi r13, -1 # return -1 if the student number doesn't exist 
        stb r13, (r15) # now store it in result 

iloop: br iloop

.data  	# the numbers that are the data 

/* result should hold the grade of the student number put into r10, or
-1 if the student number isn't found */ 

SnumberToSearch: .word 10392584

result: .byte 0
        .align 2 # assembler ensures the next piece of data is aligned on a 2^2 boundary
		
/* Snumbers is the "array," terminated by a zero of the student numbers  */
Snumbers: .word 10392584, 423195, 644370, 496059, 296800
        .word 265133, 68943, 718293, 315950, 785519
        .word 982966, 345018, 220809, 369328, 935042
        .word 467872, 887795, 681936, 0

/* Grades is the corresponding "array" with the grades, in the same order*/
Grades: .byte 99, 68, 90, 85, 91, 67, 80
        .byte 66, 95, 91, 91, 99, 76, 68  
        .byte 69, 93, 90, 72
	
	
