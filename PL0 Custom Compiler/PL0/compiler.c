// COP 3402, Spring 2020
// This code WAS written by Nikhil Mahalanobis
// This program file is the file that extends the funcitonality of the Parser,
// and where we run our PL0 compiler from.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "lexAnalyzer.h"
#include "vmach.h"
#include "parse.h"

// Converts output of lexicographical analyzer (number) to its corresponding identifier
void lexNumberToSymbolic(FILE *output)
{
  FILE *input = fopen("assets/lexeme_list.txt", "r");

  fseek(input, 0, SEEK_SET);

  while (!feof(input))
  {

    int number = -1;

    fscanf(input, "%d", &number);

    // Checking if we have a valid translation.
    if (number >= 1 && number <= 33)
    {

      int numtmp = -1;
      char *identmp = malloc(sizeof(char) * MAX_IDEN_LEN);

      fprintf(output, "%s ", enumString[number - 1]);

      // Identifier case.
      if (number == 2)
      {
        fscanf(input, "%s", identmp);

        fprintf(output, "%s ", identmp);
      }

      // Number case.
      if (number == 3)
      {
        fscanf(input, "%d", &numtmp);

        fprintf(output, "%d ", numtmp);
      }
    }
  }
  fclose(input);
  return;
}

void catFile(FILE *input, FILE *output)
{
  char c;
  c = fgetc(input);
  while (c != EOF)
  {
    fprintf(output, "%c", c);
    c = fgetc(input);
  }

  return;
}

int main(int argc, char *argv[])
{
  FILE *code = fopen("input.txt", "r");
  FILE *lexemeList = fopen("assets/lexeme_list.txt", "w");
  FILE *symbolicList = fopen("assets/lexeme_symbolic.txt", "w");
  FILE *output = fopen("output.txt", "w");


  printLexemeTable(code);
  printLexemeList(lexemeList);

  fclose(lexemeList);

  lexNumberToSymbolic(symbolicList);

  // We set flags depending on what command line arguments the user supplies, -a, -v, -l
  bool vFlag, aFlag, lFlag = false;

  // At this point, we have a lexeme list for the code and a symbolic translation
  // for said code.
  // lexeme_list.txt <- Lexeme list.
  // lexeme_symbolic.txt <- Symbolic translation

  if (argc == 1)
  {
    printf("in\nout\n");
    return 0;
  }

  for (int i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "-v") == 0)
    {
      vFlag = true;
    }

    if (strcmp(argv[i], "-a") == 0)
    {
      aFlag = true;
    }

    if (strcmp(argv[i], "-l") == 0)
    {
      lFlag = true;
    }
  }


  // If user used "-v" as a command line option, print virtual machine execution trace to file
  if (vFlag == true)
  {
    FILE *vmout = fopen("assets/vm_output.txt", "r");
    FILE *vmstack = fopen("assets/vm_stack_output.txt", "r");

    fprintf(output, "VM Trace: \n");
    catFile(vmout, output);
    fprintf(output, "\n");
    catFile(vmstack, output);
    fprintf(output, "\n\n");

    fclose(vmout);
    fclose(vmstack);

  }

  // If user used "-l" as a command line option, print list of lexemes/token to file
  if (lFlag == true)
  {
    FILE *lex = fopen("assets/lexeme_list.txt", "r");

    fprintf(output, "Lex List:\n");
    catFile(lex, output);
    fprintf(output, "\n\n");

    fclose(lex);

  }

  // If user used "-a" as a command line option, print generated assembly code (parser/codegen output) to file
  if (aFlag == true)
  {
    FILE *assembly = fopen("assets/parse_output.txt", "r");

    fprintf(output, "Assembly: \n");
    catFile(assembly, output);
    fprintf(output, "\n\n");

    //lexList = fopen("assets/lexeme_list.txt", "r");


    FILE *lex = fopen("assets/lexeme_list.txt", "r");
    fp = lex;

    fseek(lex, 0, SEEK_SET);

    program();

    fclose(assembly);
    fclose(lex);
  }
  fclose(symbolicList);
  fclose(code);
  fclose(output);
  return 0;
}
