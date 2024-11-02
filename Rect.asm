// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/4/Fill.asm

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

   @0

   D=M

   @INFINITE_LOOP

   D;JLE 

   @counter

   M=D

   @SCREEN

   D=A

   @address

   M=D

(LOOP)

   @address

   A=M

   M=-1

   @address

   D=M

   @32

   D=D+A

   @address

   M=D

   @counter

   MD=M-1

   @LOOP

   D;JGT

(INFINITE_LOOP)

   @INFINITE_LOOP

   0;JMP


