This project is a simulation of a core part of a mini processor called MySPIM. It was a project made in UCF class CDA-3103.
All of the code written by me (Nikhil Mahalanobis) is in the file "project.c"
All of the other header and code files (i.e. spimcore.c & spimcore.h) were given to us by the professor as supporting files, and were NOT written by me.

To run the program using the Linux command line, enter the follwing commands:
	
	- $gcc -o spimcore spimcore.c project.c
	- $./spimcore input_file.asc

The program should run and compile successfully. If it did, you should be prompted with the command as follows:
	
	- cmd:

The program is set to take command line arguments as characters, and will perform different actions based on them. The controls are as follows:

	-r: show register contents
	-m: show memory contents 
	-s: step forward 1 instruction
	-c: continue all instructions until there's a halt
	-h: check to see if program has halted
	-x: quit

"input_file.asc" is a test input that was provided to us in class, and "expected_output.txt" is the expected output of running the program with the given input file.
I included these if you wish to run the program yourself, and compare outputs.
