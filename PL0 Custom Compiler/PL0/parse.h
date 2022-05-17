// COP 3402, Spring 2020
// This code WAS written by Nikhil Mahalanobis
// Program for a Recursive Descent Parser w/ Itermediate Code Generation for PL0
// The parser reads the output of the Scanner (lexAnalyzer.h) and parses the lexeme tokens

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_SYMBOL_TABLE_SIZE 300
#define CODE_SIZE 200

// Struct for our symbols, each has 5 fields
typedef struct
{
  int kind; 		 // const = 1, var = 2, proc = 3
  char name[10]; // name up to 11 chars
  int val; 		   // number (ASCII value)
  int level; 		 // L level
  int addr; 		 // M address
} symbol;

// Struct for our tokens from lex analyzer
typedef struct{
  char type[3];   // Stores associated value w/ token (1 - 33)
  char value[12]; // Value for token only if its an identifier or number, otherwise holds nothing
} token;

typedef struct{
  int op; // opcode.
  int r;  // register.
  int l;  // lexicographical level
  int m;  // modifier
} code;

symbol symbolTable[MAX_SYMBOL_TABLE_SIZE]; // Symbol struct array to create the symbol table
code codeArray[CODE_SIZE]; // Array of code struct to help us hold the translated assembly code for code generation

int symbolTableCounter; // Counter for symbol table entries created
int registerPointer;    // Pointer to hold place of what register we're on
int stackPointer;       // Pointer to keep track of where we are in the stack

int currentLevel;   // Current lexicographical level
int maxLexLevel;    // Currently the max lexicographical level in our program
token currentToken; // Current token to analyze
int codePointer;    // Pointer to keep track of where in the code array we're at

FILE *fp;

// The EBNF heirarchy for PL0 programming language (from the top down)
void program(); // We start with a program
void block(); // Each program breaks down into a block followed by a "."
void statement(); // A block can have a statement inside it
void condition(); // Statements can contain conditions
int relational_op(); // Conditions can contain expressions and relational operators
void expression(); // Expressions contain terms
void term(); // Terms contain factors
void factor(); // Factors are represented with a ident OR number OR another expression


// Function for fetching next token
void getNextToken()
{
  if(fscanf(fp, "%s", currentToken.type ) != EOF)
  {
    // If token is identsym or numbersym, get token value.
    if(!strcmp(currentToken.type, "2") || !strcmp(currentToken.type, "3"))
      fscanf(fp, "%s", currentToken.value );

    // All other tokens set value to null
    else currentToken.value[0] = '\0';
  }

  // No more tokens.
  else
  {
    currentToken.type[0] = '\0'; // Set token type to null
    currentToken.value[0] = '\0'; // Set token value to null
  }
}

// Function to insert token into symbol table.
void insertSym(int Kind, char name[], int val, int address)
{
  symbolTable[symbolTableCounter].kind = Kind;
  strcpy(symbolTable[symbolTableCounter].name, name);
  symbolTable[symbolTableCounter].val = val;
  symbolTable[symbolTableCounter].addr = address;
  symbolTableCounter++;
}

// Function to print all possible errors in our compiler
void error( int n )
{
  printf("Error number ");
  switch(n)
  {

    case 1:
    printf("1.	Use = instead of :=\n");
    break;
    case 2:
    printf("2.	= must be followed by a number.\n");
    break;
    case 3:
    printf("3.	Identifier must be followed by =\n");
    break;
    case 4:
    printf("4.	const, var, procedure must be followed by identifier.\n");
    break;
    case 5:
    printf("5.	Semicolon or comma missing.\n");
    break;
    case 6:
    printf("6.	Incorrect symbol after procedure declaration.\n");
    break;
    case 7:
    printf("7.	Statement expected.\n");
    break;
    case 8:
    printf("8.	Incorrect symbol after statement part in block.\n");
    break;
    case 9:
    printf("9.	Period expected.\n");
    break;
    case 10:
    printf("10.	Semicolon between statements missing.\n");
    break;
    case 11:
    printf("11.	Undeclared identifier.\n");
    break;
    case 12:
    printf("12.	Assignment to constant or procedure is not allowed.\n");
    break;
    case 13:
    printf("13.	Assignment operator expected.\n");
    break;
    case 14:
    printf("14.	call must be followed by an identifier.\n");
    break;
    case 15:
    printf("15.	Call of a constant or variable is meaningless.\n");
    break;
    case 16:
    printf("16.	then expected.\n");
    break;
    case 17:
    printf("16.	then expected.\n");
    break;
    case 18:
    printf("18.	do expected.\n");
    break;
    case 19:
    printf("19.	Incorrect symbol following statement.\n");
    break;
    case 20:
    printf("20.	Relational operator expected.\n");
    break;
    case 21:
    printf("21.	Expression must not contain a procedure identifier.\n");
    break;
    case 22:
    printf("22.	Right parenthesis missing.\n");
    break;
    case 23:
    printf("23.	The preceding factor cannot begin with this symbol.\n");
    break;
    case 24:
    printf("24.	An expression cannot begin with this symbol.\n");
    break;
    case 25:
    printf("25.	This number is too large.\n");
    break;
    case 26:
    printf("26. Number expected.\n");
    break;
    case 27:
    printf("27, all available registers in use\n");
    break;
    case 28:
    printf("28, variable not initialized\n");
    break;
    case 29:
    printf("29, identifier expected after read/write\n");
    case 30:
    printf("30, code exceeds MAX_CODE_SIZE\n");
  }
  fclose(fp);
  // Terminate early
  exit(0);
}

void emit(int op, int R, int L, int M)
{
  if (codePointer > CODE_SIZE)
    error(25);
  else
  {
    codeArray[codePointer].op = op;
    codeArray[codePointer].r = R;
    codeArray[codePointer].l = L;
    codeArray[codePointer].m = M;
    codePointer++;
  }
}
 // program ::= block"."
void program()
{
  getNextToken();
  block();


  if (strcmp(currentToken.type, "19") != 0)
    error(9);

  printf("No errors, program is syntactically correct.\n");
}


/*
statement ::= [ ident ":=" expression
              |"begin" statement { ";" statement } "end"
              | "if" condition "then" statement
              | "while" condition "do" statement
              | "read" ident
              | "write"  ident
              | e ] .
*/
void statement()
{
  int index;
  // A flag implementation to see if we have an incorrect declaration in our syntax.
  int declaredFlag = 0;
  // Temporary index counter for checking the symbol table for our symbol
  int symbolTempIndex;

  // identifier
  if (!strcmp(currentToken.type, "2"))
  {
    // Loop that checks the symbol table for our identifier
    for (index = symbolTableCounter - 1; index >= 0; index--)
    {
      if (!strcmp(currentToken.value, symbolTable[index].name))
      {
        if (symbolTable[index].kind == 1)
          error(12);
        else if (symbolTable[index].kind == 2)
        {
          declaredFlag = 1;
          symbolTempIndex = index;
        }
      }
    }

    // If flag is set off, undeclared identsym
    if (!declaredFlag)
      error(11);

    getNextToken();
    if (strcmp(currentToken.type, "20") != 0)
      error(3);

    getNextToken();
    expression();

    // Add correct instruction to our code Array
    emit(4, registerPointer - 1, currentLevel - symbolTable[symbolTempIndex].level, symbolTable[symbolTempIndex].addr - 1);
    registerPointer--;
  }

  // Call
  else if (!strcmp(currentToken.type, "27"))
  {
    // Flag like above, to see if we have undeclared identifier.
    int callFlag = 0;
    getNextToken();

    if (strcmp(currentToken.type, "2"))
      error(14);

    // Loop to check for identifier in symbol table.
    for (index = symbolTableCounter - 1; index >= 0; index--)
    {
      if (!strcmp(currentToken.value, symbolTable[index].name))
      {
        symbolTempIndex = index;
        callFlag = 1;
      }
    }

    if (!callFlag)
      error(11);

    // If we have the corresponding proc ident, add call to the table. Else, error.
    if (symbolTable[symbolTempIndex].kind == 3)
    {
      emit(5, 0, maxLexLevel, symbolTable[symbolTempIndex].addr);
      currentLevel++;
    }
    else
      error(14);

    getNextToken();
  }

  // Begin
  else if (!strcmp(currentToken.type, "21"))
  {
    getNextToken();
    statement();

    while (!strcmp(currentToken.type, "18"))
    {
      getNextToken();
      statement();
    }

    if (strcmp(currentToken.type, "22") != 0)
    error(26);

    getNextToken();
  }

  // if-then
  else if (!strcmp(currentToken.type, "23"))
  {
    getNextToken();
    condition();

    if (strcmp(currentToken.type, "24") != 0)
    error(16);

    getNextToken();

    int cx = codePointer;
    emit(8, registerPointer - 1, 0, 0);

    statement();
    getNextToken();

    // else
    if (!strcmp(currentToken.type, "33"))
    {
      int cx2 = codePointer;

      emit (7, 0, 0, 0);
      codeArray[cx].m = codePointer;

      getNextToken();
      statement();

      codeArray[cx2].m = codePointer;

      registerPointer--;
    }

    // if-then, NOT followed by else.
    else
    {
      codeArray[cx].m = codePointer;
      registerPointer--;
    }
  }

  //while-do
  else if (!strcmp(currentToken.type, "25") != 0)
  {
    int codePointer1 = codePointer;

    getNextToken();
    condition();
    int codePointer2 = codePointer;

    emit(8, registerPointer - 1, 0, 0);

    if (strcmp(currentToken.type, "18") != 0)
    error(18);

    getNextToken();
    statement();

    emit(7, 0, 0, codePointer1);
    codeArray[codePointer2].m = codePointer;

    registerPointer--;
  }

  // read
  else if (!strcmp(currentToken.type, "32"))
  {
    getNextToken();

    if (strcmp(currentToken.type, "2") != 0)
      error(29);

    // Loop to check symbol table for identifier
    for (index = symbolTableCounter - 1; index >= 0; index--)
      if (!strcmp(currentToken.value, symbolTable[index].name))
      {
        declaredFlag = 1;
        symbolTempIndex = index;
      }

    // If flag set off, undeclared identsym
    if (!declaredFlag)
      error(11);

    // Add instruction to code array
    emit(10, registerPointer, 0, 2);

    // Store var into memory
    if (symbolTable[symbolTempIndex].kind == 2)
      emit(4, registerPointer, currentLevel - symbolTable[symbolTempIndex].level, symbolTable[symbolTempIndex].addr - 1);

    // Trying to store value in constant
    else if (symbolTable[symbolTempIndex].kind == 1)
      error(12);

    getNextToken();
  }

  // write
  else if (!strcmp(currentToken.type, "31"))
  {
    getNextToken();

    if (strcmp(currentToken.type, "2") != 0)
      error(29);

    // Loop to check symbol table for indentifier
    for (index = symbolTableCounter - 1; index >= 0; index--)
    if (!strcmp(currentToken.value, symbolTable[index].name))
    {
      if (symbolTable[index].kind == 1 || symbolTable[index].kind == 2)
      {
        declaredFlag = 1;
        symbolTempIndex = index;
      }
    }

    // undeclared identifier flag
    if (!declaredFlag)
      error(11);

    // Get var from memory and write to screen
    if (symbolTable[symbolTempIndex].kind == 2)
    {
      emit(3, registerPointer, currentLevel - symbolTable[symbolTempIndex].level, symbolTable[symbolTempIndex].addr - 1);
      emit(9, registerPointer, 0, 1);
    }

    getNextToken();
  }
}

// block ::= const-declaration var-declaration statement.
void block()
{
  char name[12]; // Temp string to hold associated identifier of symbol
  int val;      // Temp int to hold value of symbol

  int jumpAddress = codePointer; // Variable to hold the location of jump address we need to go back to
  int offset = 4;               // "Offset" of space for when we declare new procedure.
  stackPointer = 5;             // Update stack pointer after adding new procedure.
  int procIndex;                // Temp index to hold place of procedure in symbol table.

  // Constant
  if (!strcmp(currentToken.type, "28"))
  {
    do {

      getNextToken();
      if (strcmp(currentToken.type, "2") != 0)
        error(4);

      strcpy(name, currentToken.value);

      getNextToken();

      if (strcmp(currentToken.type, "9") != 0)
        error(3);

      getNextToken();

      if (strcmp(currentToken.type, "3") != 0)
        error(2);

      val = atoi(currentToken.value);

      insertSym(1, name, val, 0);

      getNextToken();
    } while (!strcmp(currentToken.type, "17"));

    if (strcmp(currentToken.type, "18") != 0)
      error(5);

    getNextToken();
  }

  // Variable
  if (!strcmp(currentToken.type, "29"))
  {
    do {

      getNextToken();

      if (strcmp(currentToken.type, "2") != 0)
        {
          printf("....\n");
          error(4);
        }

      strcpy(name, currentToken.value);

      insertSym(2, name, 0, stackPointer);

      getNextToken();
    } while (!strcmp(currentToken.type, "17"));

    if (strcmp(currentToken.type, "18") != 0)
      error(5);
    getNextToken();
  }

  // Procedure
  while (!strcmp(currentToken.type, "30"))
  {
    getNextToken();

    // If we have missing ident, error.
    if (strcmp(currentToken.type, "2") != 0)
      error(4);

    strcpy(name, currentToken.value);

    // Add ident to symbol table, keeps track of index in table.
    insertSym(3, name, 0, 0);
    procIndex = symbolTableCounter - 1;
    symbolTable[procIndex].level = maxLexLevel;
    symbolTable[procIndex].addr = jumpAddress + 1;

    getNextToken();

    // Semicolon expected check, if not return error.
    if (strcmp(currentToken.type, "18") != 0)
      error(17);

    getNextToken();

    // Increase max lexicographical level by one.
    maxLexLevel++;

    block();

    if (strcmp(currentToken.type, "18") != 0)
      error(17);

    getNextToken();
  }

  // Add INC to symbol table if we have a procedure.
  codeArray[jumpAddress].m = codePointer;
  emit(6, 0, 0, offset);

  statement();

  // Add RTN to symbol table when we return from call.
  emit(2, 0, 0, 0);
  currentLevel--;

  statement();
}


// expression ::= [ "+"|"-"] term { ("+"|"-") term}.
void expression()
{
  char addop[10]; // Temp string to hold current token op

  if (!strcmp(currentToken.type, "4") || !strcmp(currentToken.type, "5"))
  {
    strcpy(addop, currentToken.type);
    getNextToken();
    term();
    if (!strcmp(addop, "5"))
      emit(11, registerPointer - 1, registerPointer - 1, 0);
  }
  else
    term();

  while (!strcmp(currentToken.type, "4") || !strcmp(currentToken.type, "5"))
  {
    strcpy(addop, currentToken.type);
    getNextToken();
    term();
    if (!strcmp(addop, "4"))
    {
      emit(12, registerPointer - 2, registerPointer - 2, registerPointer - 1);
      registerPointer--;
    }
    else
    {
      emit(13, registerPointer - 2, registerPointer - 2, registerPointer - 1);
      registerPointer--;
    }
  }
}

// condition ::= "odd" expression | expression  rel-op  expression.
void condition()
{
  if (!strcmp(currentToken.type, "8"))
  {
    getNextToken();
    expression();
    emit(16, registerPointer - 1, registerPointer - 1, 0);
  }
  else
  {
    expression();

    // Made relational_op() function that checks to see if currentToken is a relationl operator, returns 0 if it isn't
    int op = relational_op();

    if (op == 0)
      error(20);

    getNextToken();
    expression();

    emit(op, registerPointer - 2, registerPointer - 2, registerPointer - 1);
    registerPointer--;
  }
}

// rel-op ::= "="|“<>"|"<"|"<="|">"|">=".
// Function that checks to see if currentToken is a relational operator or not.
// If it is, then return the associated assembly OP code value of that token, otherwise return 0
int relational_op()
{
  if(!strcmp( currentToken.type, "9"))
    return 18;
  else if(!strcmp( currentToken.type, "10"))
    return 19;
  else if(!strcmp( currentToken.type, "11"))
    return 20;
  else if(!strcmp( currentToken.type, "12" ))
    return 21;
  else if(!strcmp( currentToken.type, "13"))
    return 22;
  else if(!strcmp( currentToken.type, "14"))
    return 23;

  else
    return 0;
}


// term ::= factor {("*"|"/") factor}.
void term()
{
  // Temp String to hold token type
  char mulop[10];

  factor();
  while (!strcmp(currentToken.type, "6") || !strcmp(currentToken.type, "7"))
  {
    strcpy(mulop, currentToken.type);
    getNextToken();
    factor();
    if (!strcmp(mulop, "6"))
    {
      emit(14, registerPointer - 2, registerPointer  -2, registerPointer - 1);
      registerPointer--;
    }
    else
    {
      emit(15, registerPointer - 2, registerPointer - 2, registerPointer - 1);
      registerPointer--;
    }
  }
}


// factor ::= ident | number | "(" expression ")".
void factor()
{
  int index;
  // Same as before, flag to help us track undeclared identifier error
  int declaredFlag = 0;
  // Same as before, temp index to keep track of where in symbol table we are
  int symbolTempIndex;


  if (!strcmp(currentToken.type, "2"))
  {
    // Loop to search through symbol table
    for (index = symbolTableCounter - 1; index >= 0; index--)
      if (!strcmp(currentToken.value, symbolTable[index].name))
      {
        declaredFlag = 1;
        symbolTempIndex = index;
      }

    // If flag set off, undeclared identifier error
    if (!declaredFlag)
      error(11);

    // If we have a var, load it from memory
    if (symbolTable[symbolTempIndex].kind == 2)
      emit(3, registerPointer, currentLevel - symbolTable[symbolTempIndex].level, symbolTable[symbolTempIndex].addr - 1);

    // Else if we have a constant, load from symbol table.
    else if (symbolTable[symbolTempIndex].kind == 1)
      emit(1, registerPointer, 0, symbolTable[symbolTempIndex].val);

    registerPointer++;
    getNextToken();
  }

  else if (!strcmp(currentToken.type, "3"))
  {
    emit(1, registerPointer, 0, atoi(currentToken.value));
    registerPointer++;
    getNextToken();
  }

  else if (!strcmp(currentToken.type, "15"))
  {
    getNextToken();
    expression();

    if (strcmp(currentToken.type, "16") != 0)
      error(22);

    getNextToken();
  }

  else
  {
    error(23);
  }
}
