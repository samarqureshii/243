.global _start
	.equ KEY_BASE, 0xFF200050
	.equ LEDs, 0xFF200000

_start:
    movia r8,KEY_BASE	# set r8 to base KEY port
    movia r9,LEDs		# set r9 to base of LEDR port
	movi r12, 0			#initialize led display value
	movi r5, 15
	movi r6, 1
	

poll:
	#check each key
	ldwio r11, 0(r8)
	movi r4, 1
	beq r11, r4, key_zero
	movi r4, 2
	beq r11, r4, key_one
	movi r4, 4
	beq r11, r4, key_two
	movi r4, 8
	beq r11, r4, key_three
	
	br poll
	
key_zero:
	ldwio r11, 0(r8)
	bne r11, r0, key_zero
	
	movi r12, 1
	stwio r12, (r9)
	
	br poll
	
key_one:
	ldwio r11, 0(r8)
	bne r11, r0, key_one
	
	beq r12, r5, poll
	addi r12, r12, 1
	
	stwio r12, (r9)
	
	br poll
	
key_two:
	ldwio r11, 0(r8)
	bne r11, r0, key_two
	
	ble r12, r6, poll
	subi r12, r12, 1
	
	stwio r12, (r9)
	
	br poll
	
key_three:
	ldwio r11, 0(r8)
	bne r11, r0, key_three
	
	movi r12, 0
	stwio r12, (r9)
	
	br poll