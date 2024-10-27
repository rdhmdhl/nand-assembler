// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

@SCREEN // STARTING ADDRESS OF SCREEN
D=A
// addr = base address of first screen row
@addr
M=D

@256
D=A
@row_counter
M=D

@32
D=A
@words_per_row
M=D

(LOOP_WHITE)
	// IF KEYBOARD IS PRESSED, GOTO END_SCREEN FOR INSTRUCTIONS
	@KBD
	D=M
	@END_SCREEN
	D;JGT
	
	// IF KEYBOARD IS NOT PRESSED, CONTINUE TO WHITE SCREEN

	@addr
	A=M // A = CURRENT WORD ADDRESS
	M=0 // SET THE CURRENT WORD TO WHITE, 16 bits

	// SELECT THE NEXT ADDRESS OF THE NEXT 16 BITS 
	@addr
	D=M
	D=D+1
	@addr
	M=D

	// DECREASE THE WORD COUNT IN THE ROW 
	@words_per_row
	D=M
	D=D-1 
	@words_per_row
	M=D
	
 	@LOOP_WHITE
	D;JGT // IF WORD COUNT > 0, LOOP AGAIN TO FILL OUT ROW 

	// NEXT ROW, AS WORD COUNT = 0 IN CURRENT ROW 

	// RESET WORDS PER ROW
	@32
	D=A
	@words_per_row
	M=D

	// DECREASE ROW COUNT
	@row_counter
	D=M
	D=D-1
	@row_counter
	M=D

	// JUMP BACK TO LOOP IF ROW_COUNTER > 0 TO CONTINUE
	@LOOP_WHITE
	D;JGT
	 
	// JUMP TO END_SCREEN IF NO MORE ROWS
	@END_SCREEN
	D;JEQ


(LOOP_BLACK)
	// IF KEYBOARD IS NOT PRESSED, GOTO END_SCREEN FOR INSTRUCTIONS
	@KBD
	D=M
	@END_SCREEN
	D;JEQ


	@addr
	A=M // A = CURRENT WORD ADDRESS
	M=-1 // SET THE CURRENT WORD TO BLACK, 16 bits

	// SELECT THE NEXT ADDRESS OF THE NEXT 16 BITS 
	@addr
	D=M
	D=D+1
	@addr
	M=D

	// DECREASE THE WORD COUNT IN THE ROW 
	@words_per_row
	D=M
	D=D-1 
	@words_per_row
	M=D
	
 	@LOOP_BLACK
	D;JGT // IF WORD COUNT > 0, LOOP AGAIN TO FILL OUT ROW 

	// NEXT ROW, AS WORD COUNT = 0 IN CURRENT ROW 

	// RESET WORDS PER ROW
	@32
	D=A
	@words_per_row
	M=D

	// DECREASE ROW COUNT
	@row_counter
	D=M
	D=D-1
	@row_counter
	M=D
	// JUMP BACK TO LOOP IF ROW_COUNTER > 0
	@LOOP_BLACK
	D;JGT
	 
	// JUMP TO END_SCREEN IF NO MORE ROWS
	@END_SCREEN
	D;JEQ

(END_SCREEN)
	// RESET THE WORD PER ROW COUNT
	@32
	D=A
	@words_per_row
	M=D

	// RESET THE SCREEN ADDRESS 
	@SCREEN 
	D=A
	// base address of first screen row
	@addr	
	M=D

	// RESET ROW COUNTER
	@256
	D=A
	@row_counter
	M=D
	
	// IF KEYBOARD IS PRESSED, GOTO LOOP_BLACK
	@KBD
	D=M
	@LOOP_BLACK
	D;JGT
	
	// IF KEYBOARD IS NOT PRESSED, GOTO LOOP_WHITE
	@KBD
	D=M
	@LOOP_WHITE
	D;JEQ
		
	// INFINITE LOOP TO STOP THE PROGRAM 
	@END_SCREEN
	0;JMP





