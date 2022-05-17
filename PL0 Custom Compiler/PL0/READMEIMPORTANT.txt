This project is a full compiler (a virtual stack machine, a lexicographcial analyzer, and a parser) for the small coding language PL0.
All code written in "vmach.h", "lexAnalyzer.h", "parse.h", and "compiler.c" were written by me (Nikhil Mahalanobis)
All other files (i.e. input.txt, output.txt, errors.txt, and assets folder) are supporting files that the program requires inorder to run. 

Necesary files that come with this program include:
	- input.txt
	- output.txt
	- compiler.c
	- vmach.h
	- parse.h
	- lexAnalyzer.h
	- assets folder

readme.txt:
	This file contains instructions on how to run this program.

input.txt
	This file contains the sample input that is used to run the program.
	**NOTE: If you would like to run your own input, please open this file, clear it, and write in your own data.

output.txt
	This file contains the current sample output to the sample input (input.txt). It displays:
	- Source program in assembly (with VM stack trace)
	- Lexical Table
	- Lexical List
	- Stack contents

compiler.c
	This file is the source code for the lexical analyzer for PL/0.
	It runs and compiles all parts of the compiler (vmach.h, lexAnalyzer.h, parse.h)

assets folder
	This folder has text files needed to run the program.

** IMPORTANT **
In order for this program to run successfully, all files (excluding readme.txt) must be present in the same directory as compiler.c
	- To compile this program on Linux Command Line, please change directories (Use cd command in linux) to go to the directory
	of the compiler.c and other txt files and perform the following commands:
		> gcc compiler.c -o compile
		> ./compile -l -a -v
The command line arguments that the program can take represent the following:
	-a: show the generate assembly code (scanner/lexAnalyzer.h output) to output.txt
	-l: show the lexeme/token list (parser/parse.h output) to output.txt
	-v: print VM execution trace (VM/vmach.h output) to output.txt
	NOTE: if you run the program without any of the command line arguments, it will still run, but no information will be printed to the output file

If all necessary files are present (all text files), then you will need to open output.txt for the output of the program.

These files come with data inside from the test case given in the assignment. If you would like to, you may wipe the output text files.

PLEASE VIEW THE TEXT FILES BY USING WINDOWS NOTEPAD AS THE INDENTATION MAY SEEM OFF IF USED WITH ANOTHER PROGRAM.

****************
