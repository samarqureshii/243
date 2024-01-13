/* Program to add the numbers 2 and 3 (placed in register 8 and 9)
 and put the result into register r10 */
.global _start
_start:
      movi r8,2
      movi r9,3
      add r10, r8, r9
   done: br done
/* r8 <- 2 */
/* r9 <- 3 */
      /* r10 <- r8 + r9 */
   /* infinite loop */