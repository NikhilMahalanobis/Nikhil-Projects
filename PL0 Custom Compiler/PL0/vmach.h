// COP 3402, Spring 2020
// This code WAS written by Nikhil Mahalanobis
// implementation of a PL0 virtual machine, the P-Machine
// The P-Machine is a stack machine

#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_HEIGHT 40
#define MAX_CODE_LENGTH 200
#define MAX_LEXI_LEVEL 3
#define REGISTER_LENGTH 8

// Instruction format, each contains 4 fields:
// op - the operation code
// r - register
// l -lexicographical level (register value in arithmetic operations)
// m - Depends on the instruction: A number, program address, data address, register value
typedef struct instruction
{
  int op;
  int r;
  int l;
  int m;
}instruction;

// This function prints out the register defined below.
void printRegister(int registerArray[], FILE *stackOut)
{
  int i;

  for (i = 0; i < REGISTER_LENGTH; i++)
  {
    fprintf(stackOut, "%d ", registerArray[i]);
  }
  fprintf(stackOut, "\n");
}

// This funtion returns the length of the text array that's being used.
int getTextLength(instruction t[])
{
  int i = 0;
  while (t[i].op != 0)
    i++;

  return i;
}

// All of our instructions in the ISA
char *translation[25] = {"NULL", "lit", "rtn", "lod", "sto", "cal", "inc", "jmp", "jpc", "sio", "sio", "sio"
                    ,"neg", "add", "sub", "mul", "div", "odd", "mod", "eql", "neq", "lss", "leq",
                    "gtr", "geq"};

// Initialize stack and register to 0 as per the document.
int stack[MAX_STACK_HEIGHT] = {0};
int registerFile[REGISTER_LENGTH] = {0};

// Text with length 200.
instruction text[MAX_CODE_LENGTH];
instruction ir;

// Initialize our other variables to 0.
int sp = 0;
int pc = 0;
int haltFlag = 0;

// Base pointer is initialized to 1.
int bp = 1;

// Prints the stack instructions as output to text file
void printStack(int stack[], int n, FILE *stackOut)
{
  int i;
  int barIndex = -1;

  fprintf(stackOut, " > Stack: ");
  if (n == 0)
  {
    fprintf(stackOut, "<empty stack>\n\n");
    return;
  }

  if (sp - bp + 1 > 1)
  {
    barIndex = sp - bp + 1;
  }

  for (i = 1; i < n + 1; i++)
  {
    fprintf(stackOut, "%d ", stack[i]);
    if (i == barIndex && n > barIndex)
    {
      fprintf(stackOut, "| ");
    }
  }
  fprintf(stackOut, "\n\n");
}

// Base function
int base(int l, int base)
{
  // Find base L levels down.
  int b1;

  b1 = base;

  while (l > 0)
  {
    b1 = stack[b1 + 1];
    l--;
  }
  return b1;
}

void vm_execute()
{
  // Counter for loop
  int i = 0;

  FILE *input = fopen("assets/parse_output.txt", "r");
  FILE *output = fopen("assets/vm_output.txt", "w");
  FILE *stack_output = fopen("assets/vm_stack_output.txt", "w");


  while (!feof(input))
  {
    // Reading in code from text file to put in code stack.
    fscanf(input, "%d", &text[i].op);
    fscanf(input, "%d", &text[i].r);
    fscanf(input, "%d", &text[i].l);
    fscanf(input, "%d", &text[i].m);

    i++;
  }
  int j;

  fprintf(output, "Line\tOP\tR\tL\tM\n");
  fprintf(stack_output, "\t\tpc\tbp\tsp\tRegisters\n");
  fprintf(stack_output, "Initial values\t%d\t%d\t%d\t", pc, bp, sp);
  printRegister(registerFile, stack_output);
  fprintf(stack_output, " > Stack: ");

  for (j = 0; j < MAX_STACK_HEIGHT; j++)
  {
    fprintf(stack_output, "%d ", stack[j]);
  }
  fprintf(stack_output, "\n\n");

  int lengthText = getTextLength(text);

  // Writing data to output.txt.
  for (j = 0; j < lengthText; j++)
  {
    fprintf(output, "%d\t%s\t%d\t%d\t%d\n", j, translation[text[j].op], text[j].r, text[j].l, text[j].m);
  }

  // Re-using i for line counter.
  i = 0;

  while (haltFlag == 0)
  {
    ir = text[pc];

    // Dependent on which instruction we read, case for each one.
    switch(ir.op)
    {
      // LIT
      // Push constant literal value M onto stack
      case 1:
      fprintf(stack_output, "%02d %s %d %d %d", pc, translation[ir.op], ir.r, ir.l, ir.m);
      pc++;

      registerFile[ir.r] = ir.m;

      fprintf(stack_output, "\t%d\t%d\t%d\t", pc, bp, sp);
      printRegister(registerFile, stack_output);
      printStack(stack, sp, stack_output);

      break;

      // RTN
      // Return from subroutine
      case 2:
      fprintf(stack_output, "%02d %s %d %d %d", pc, translation[ir.op], ir.r, ir.l, ir.m);
      pc++;


      sp = bp - 1;
      bp = stack[sp + 3];
      pc = stack[sp + 4];

      fprintf(stack_output, "\t%d\t%d\t%d\t", pc, bp, sp);
      printRegister(registerFile, stack_output);
      printStack(stack, sp, stack_output);

      break;

      // LOD
      // Push from location at offset M in frame L levels down
      case 3:
      fprintf(stack_output, "%02d %s %d %d %d", pc, translation[ir.op], ir.r, ir.l, ir.m);
      pc++;


      registerFile[ir.r] = stack[base(ir.l, bp) + ir.m];

      fprintf(stack_output, "\t%d\t%d\t%d\t", pc, bp, sp);
      printRegister(registerFile, stack_output);
      printStack(stack, sp, stack_output);

      break;

      // STO
      // Store in location at offest M in frame L levels down
      case 4:
      fprintf(stack_output, "%02d %s %d %d %d", pc, translation[ir.op], ir.r, ir.l, ir.m);
      pc++;


      stack[base(ir.l, bp) + ir.m] = registerFile[ir.r];

      fprintf(stack_output, "\t%d\t%d\t%d\t", pc, bp, sp);
      printRegister(registerFile, stack_output);
      printStack(stack, sp, stack_output);

      break;

      // CAL
      // Call procedure at M
      case 5:
      fprintf(stack_output, "%02d %s %d %d %d", pc, translation[ir.op], ir.r, ir.l, ir.m);
      pc++;


      stack[sp + 1] = 0;
      stack[sp + 2] = base(ir.l, bp);
      stack[sp + 3] = bp;
      stack[sp + 4] = pc;
      bp = sp + 1;
      pc = ir.m;

      fprintf(stack_output, "\t%d\t%d\t%d\t", pc, bp, sp);
      printRegister(registerFile, stack_output);
      printStack(stack, sp, stack_output);

      break;

      // INC
      // Increment the stack pointer (sp) by M
      case 6:
      fprintf(stack_output, "%02d %s %d %d %d", pc, translation[ir.op], ir.r, ir.l, ir.m);

      pc++;
      sp = sp + ir.m;

      fprintf(stack_output, "\t%d\t%d\t%d\t", pc, bp, sp);
      printRegister(registerFile, stack_output);
      printStack(stack, sp, stack_output);

      break;

      // JMP
      // Jump the pc to M, pc = M
      case 7:
      fprintf(stack_output, "%02d %s %d %d %d", pc, translation[ir.op], ir.r, ir.l, ir.m);
      pc++;
      pc = ir.m;

      fprintf(stack_output, "\t%d\t%d\t%d\t", pc, bp, sp);
      printRegister(registerFile, stack_output);
      printStack(stack, sp, stack_output);

      break;

      // JPC
      // Jump to M if top of stack == 0 and decrement sp by 1
      case 8:
      fprintf(stack_output, "%02d %s %d %d %d", pc, translation[ir.op], ir.r, ir.l, ir.m);
      pc++;

      if (registerFile[ir.r] == 0)
        pc = ir.m;

      fprintf(stack_output, "\t%d\t%d\t%d\t", pc, bp, sp);
      printRegister(registerFile, stack_output);
      printStack(stack, sp, stack_output);

      break;

      // SIO 1
      // Write the designated register to the screen
      case 9:
      fprintf(stack_output, "%02d %s %d %d %d", pc, translation[ir.op], ir.r, ir.l, ir.m);
      pc++;

      printf("%d\n", registerFile[ir.r]);

      fprintf(stack_output, "\t%d\t%d\t%d\t", pc, bp, sp);
      printRegister(registerFile, stack_output);
      printStack(stack, sp, stack_output);

      break;

      // SIO 2
      // Read input from user and store in register
      case 10:
      fprintf(stack_output, "%02d %s %d %d %d", pc, translation[ir.op], ir.r, ir.l, ir.m);
      pc++;

      printf("Enter the number you wish to store in the register: ");
      scanf("%d", &registerFile[ir.r]);

      fprintf(stack_output, "\t%d\t%d\t%d\t", pc, bp, sp);
      printRegister(registerFile, stack_output);
      printStack(stack, sp, stack_output);

      break;

      // SIO 3
      // End of program
      case 11:
      fprintf(stack_output, "%02d %s %d %d %d", pc, translation[ir.op], ir.r, ir.l, ir.m);
      pc++;

      haltFlag = 1;

      fprintf(stack_output, "\t%d\t%d\t%d\t", pc, bp, sp);
      printRegister(registerFile, stack_output);
      printStack(stack, sp, stack_output);

      break;

      // NEG
      // Negative calculation, -stack[sp]
      case 12:
      fprintf(stack_output, "%02d %s %d %d %d", pc, translation[ir.op], ir.r, ir.l, ir.m);
      pc++;

      registerFile[ir.r] = - registerFile[ir.r];

      fprintf(stack_output, "\t%d\t%d\t%d\t", pc, bp, sp);
      printRegister(registerFile, stack_output);
      printStack(stack, sp, stack_output);

      break;

      // ADD
      // Addition calculation
      case 13:
      fprintf(stack_output, "%02d %s %d %d %d", pc, translation[ir.op], ir.r, ir.l, ir.m);
      pc++;

      registerFile[ir.r] = registerFile[ir.l] + registerFile[ir.m];

      fprintf(stack_output, "\t%d\t%d\t%d\t", pc, bp, sp);
      printRegister(registerFile, stack_output);
      printStack(stack, sp, stack_output);

      break;

      // SUB
      // Subtraction calculation
      case 14:
      fprintf(stack_output, "%02d %s %d %d %d", pc, translation[ir.op], ir.r, ir.l, ir.m);
      pc++;

      registerFile[ir.r] = registerFile[ir.l] - registerFile[ir.m];

      fprintf(stack_output, "\t%d\t%d\t%d\t", pc, bp, sp);
      printRegister(registerFile, stack_output);
      printStack(stack, sp, stack_output);

      break;

      // MUL
      // Multiplication calculation
      case 15:
      fprintf(stack_output, "%02d %s %d %d %d", pc, translation[ir.op], ir.r, ir.l, ir.m);
      pc++;

      registerFile[ir.r] = registerFile[ir.l] * registerFile[ir.m];

      fprintf(stack_output, "\t%d\t%d\t%d\t", pc, bp, sp);
      printRegister(registerFile, stack_output);
      printStack(stack, sp, stack_output);

      break;

      // DIV
      // Division calculation
      case 16:
      fprintf(stack_output, "%02d %s %d %d %d", pc, translation[ir.op], ir.r, ir.l, ir.m);
      pc++;

      registerFile[ir.r] = registerFile[ir.l] * registerFile[ir.m];

      fprintf(stack_output, "\t%d\t%d\t%d\t", pc, bp, sp);
      printRegister(registerFile, stack_output);
      printStack(stack, sp, stack_output);

      break;

      // ODD
      // Odd check calculation using modulus
      case 17:
      fprintf(stack_output, "%02d %s %d %d %d", pc, translation[ir.op], ir.r, ir.l, ir.m);
      pc++;

      registerFile[ir.r] = registerFile[ir.r] % 2;

      fprintf(stack_output, "\t%d\t%d\t%d\t", pc, bp, sp);
      printRegister(registerFile, stack_output);
      printStack(stack, sp, stack_output);

      break;

      // MOD
      // Modulus calculation
      case 18:
      fprintf(stack_output, "%02d %s %d %d %d", pc, translation[ir.op], ir.r, ir.l, ir.m);
      pc++;

      registerFile[ir.r] = registerFile[ir.l] % registerFile[ir.m];

      fprintf(stack_output, "\t%d\t%d\t%d\t", pc, bp, sp);
      printRegister(registerFile, stack_output);
      printStack(stack, sp, stack_output);

      break;

      // EQL
      // Equality comparision calculation
      case 19:
      fprintf(stack_output, "%02d %s %d %d %d", pc, translation[ir.op], ir.r, ir.l, ir.m);
      pc++;

      if (registerFile[ir.l] == registerFile[ir.m])
        registerFile[ir.r] = 1;
      else
        registerFile[ir.r] = 0;

      fprintf(stack_output, "\t%d\t%d\t%d\t", pc, bp, sp);
      printRegister(registerFile, stack_output);
      printStack(stack, sp, stack_output);

      break;

      // NEQ
      // Not-equal calculation
      case 20:
      fprintf(stack_output, "%02d %s %d %d %d", pc, translation[ir.op], ir.r, ir.l, ir.m);
      pc++;


      if (registerFile[ir.l] != registerFile[ir.m])
        registerFile[ir.r]  = 1;
      else
        registerFile[ir.r] = 0;

      fprintf(stack_output, "\t%d\t%d\t%d\t", pc, bp, sp);
      printRegister(registerFile, stack_output);
      printStack(stack, sp, stack_output);

      break;

      // LSS
      // Less than calculation
      case 21:
      fprintf(stack_output, "%02d %s %d %d %d", pc, translation[ir.op], ir.r, ir.l, ir.m);
      pc++;

      if (registerFile[ir.l] < registerFile[ir.m])
        registerFile[ir.r]  = 1;
      else
        registerFile[ir.r] = 0;

      fprintf(stack_output, "\t%d\t%d\t%d\t", pc, bp, sp);
      printRegister(registerFile, stack_output);
      printStack(stack, sp, stack_output);

      break;

      // LEQ
      // Less than or equal to calculation
      case 22:
      fprintf(stack_output, "%02d %s %d %d %d", pc, translation[ir.op], ir.r, ir.l, ir.m);
      pc++;

      if (registerFile[ir.l] <= registerFile[ir.m])
        registerFile[ir.r]  = 1;
      else
        registerFile[ir.r] = 0;

      fprintf(stack_output, "\t%d\t%d\t%d\t", pc, bp, sp);
      printRegister(registerFile, stack_output);
      printStack(stack, sp, stack_output);

      break;

      // GTR
      // Greater than calculation
      case 23:
      fprintf(stack_output, "%02d %s %d %d %d", pc, translation[ir.op], ir.r, ir.l, ir.m);
      pc++;

      if (registerFile[ir.l] > registerFile[ir.m])
        registerFile[ir.r]  = 1;
      else
        registerFile[ir.r] = 0;

      fprintf(stack_output, "\t%d\t%d\t%d\t", pc, bp, sp);
      printRegister(registerFile, stack_output);
      printStack(stack, sp, stack_output);

      break;

      // GEQ
      // Greater than or equal to calculation
      case 24:
      fprintf(stack_output, "%02d %s %d %d %d", pc, translation[ir.op], ir.r, ir.l, ir.m);
      pc++;


      if (registerFile[ir.l] >= registerFile[ir.m])
        registerFile[ir.r]  = 1;
      else
        registerFile[ir.r] = 0;

      fprintf(stack_output, "\t%d\t%d\t%d\t", pc, bp, sp);
      printRegister(registerFile, stack_output);
      printStack(stack, sp, stack_output);

      break;

      default:
      haltFlag = 1;
      break;
    }

    i++;
  }

  fclose(input);
  fclose(output);
  fclose(stack_output);
  return;
}
