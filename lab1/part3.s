
/*Write a Nios II Assembly language program that computes the sum of the numbers from 1 to 30,
; in a loop that explicitly computes the sum (i.e. don’t use a formula to compute the sum, use a loop). 
; The sum must be placed into register r12 when the program is finished, and the program should go into 
; an infinite loop when it is done, as shown in Figure 1 at the bottom. Test your program by compiling and running it on CPUlator.

; one 5 bit register to be the iterator
; another register to store the sum */


.global _start
_start:
    movi r8, 0  /*we will use r8 as the iterator */
    movi r9, 29  /* r9 holds the value 30 to compare with r8 */
    movi r12, 0  /*we will use r12 to keep track of the sum*/

myloop:
    addi r8, r8, 1  /*increase iterator*/
    add r12, r12, r8  /*add iterator to the sum*/
    ble r8, r9, myloop /*loop back if the iterator is less than or equal to 30*/
.equ    LEDs, 0xFF200000
    movia r25, LEDs
    stwio r12, (r25)

done: br done /*; infinite loop*/

 