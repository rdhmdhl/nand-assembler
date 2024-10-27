// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.

// change every pixel on the screen to black

// used for counting loop iterations
@R1
D=M

(LOOP)
	// if counter reaches 0, break out of the loop
	@END
	D;JEQ

	// logic for adding R0 + R0 
	@R0
	D=M
	@R2
	M=D+M

	// logic for decreasing iteration count
	@R1
	M=M-1
	D=M

	// goto loop if D > 0
	@LOOP	
	D;JGT
	
(END)
	@R2
	D=M
	@OUTPUT

	@END
	0;JMP

(OUTPUT)
	M=D

