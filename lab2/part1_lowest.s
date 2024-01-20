# Change the program so that it finds the lowest number, rather than the highest.
# Run it and debug it using the same numbers that you chose above.

.global _start
_start: movia r8, result # moves the address of the label result into r8 (this is where the final result or largest number will be stored)
	ldw r9,4(r8)	# loads a word from memory into register r9. word is located at an offset of 4 bytes from the address in r8, 
					# and its expected to contain the number of elements in the dataset 
					# n is located right after result in memory, so we can just add 4 to the current memory address

	movia r10, numbers  # the address of the numbers is in r10
	# starting address of the array of numbers to be processed
	# this will act as a pointer to the current index in the label numbers we are at
	
	
/* keep largest number so far in r11 */

	ldw	r11,(r10) # load word from memory address in r10 into r11
	# loads the first number from the array into register r11
	# this register will be used to keep track of the current lowest number when we iterate through the numbers label
	
/* loop to search for biggest number */

loop: subi r9, r9, 1 # decrements a counter in r9 which initially contains the number of elements in the array
       ble r9, r0, finished # checks if the counter has reached 0, if it has, then we branch to the finished label to store the answer into result
	   
	   addi r10,r10,4   # add 4 to pointer to the numbers to point to next one
		# this is so we can iterate through the "elements" in numbers label
	   
	   ldw  r12, (r10)  # load the next number into r12
	   # load the data at memory r10 into r12 
	   # r12 is used as the current number in the numbers label we are at
	   
	   ble  r11, r12, loop  # if the current lowest is still lowest, go to loop
	   # atp, we still have not found the lowest number
	   
	   mov r11,r12   # otherwise new number is lowest, put it into r11
	   br  loop


finished: stw r11,(r8)    # store the answer into result

.equ    LEDs, 0xFF200000
    movia r25, LEDs
    stwio r11, (r25)


iloop: br iloop

result: .word 0
n:	.word 15 # number of words in the numbers label
numbers: .word 8,3,5,7,9,11,13,40,58,33,94,85,19,21,93 

	
	