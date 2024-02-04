# displays binary number on the 10 LEDs

# When no KEY is pressed, data register provides 0, and when you press KEYi, 
# Data register provides the value 1 in bit position i 

# Once button-press is detected, wait until the button is released 

# Do not use interrupt mask or edge capture registers for this part 
.global _start
_start:
.equ KEY_BASE, 0xff200050
    movia r8, KEY_BASE

movi r15, 0 # let 15 be the current number on the LEDs
movi r20, 0x1
movi r21, 0x2
movi r22, 0x4
movi r23, 0x8
movi r24, 0xf


poll: # polling loop to wait for any of the possible KEYs to repond 
    # load the data register and individually mask each of the 4 possible bits 
    ldwio r9, (r8)

    # check for KEY0
    and r10, r9, r20
    beq r10, r20, KEY0 # if bitwise AND is 0x1, then branch to key0

    # check for KEY1
    and r11, r9, r21
    beq r11, r21, KEY1

    # check for KEY2
    and r12, r9, r22
    beq r12, r22, KEY2

    # check for KEY3
    and r13, r9, r23
    beq r13, r23, KEY3

    br poll # keep polling if the result of the bitwise AND operation was 0

    # in the case they do respond, branch to any of the following:
    KEY0: # if KEY0 is pressed, set the binary number on LEDs to be b'1
        mov r16, r20
        call Delay

        movi r15, 1
        br updateLEDs

    KEY1: # if KEY1 is pressed, increment the displayed number, but don't let it go above b'1111 (d'15)
        mov r16, r21
        call Delay

        beq r15, r24, maxNum # we have hit the max number
        addi r15, r15, 1 # normal incrementation
        br updateLEDs

        maxNum:
            mov r15, r24
            br updateLEDs

    KEY2: # if KEY2 is pressed, decrement the number, but don't let it go below b'1
        mov r16, r22
        call Delay

        beq r15, r20, minNum
        beq r15, r0, minNum2

        subi r15, r15, 1
        br updateLEDs

        minNum:
            mov r15, r20
            br updateLEDs
        minNum2:
            mov r15, r0
            br updateLEDs

    KEY3: # if KEY3 is pressed, it will reset/blank the display
        mov r16, r23
        call Delay

        movi r15, 0
        br updateLEDs

    # update the LED display
    updateLEDs:
        .equ    LEDs, 0xFF200000
        movia r18, LEDs    
        stwio r15, (r18)  
        br poll

# delay loop subroutine that waits until the i-th bit in the data register goes to zero, in which, the user has released the button 
Delay:
    ldwio r9, (r8)
    and r14, r9, r16 # where r16 is the bit we want to extract, and r14 is where we will store the result and r9 is current data register 
    bne r14, r0, Delay
    ret # when the user finally released the button 

    