// COP 3402, Spring 2020
// This code WAS written by Nikhil Mahalanobis
// Program for the lexicographical analyzer for our PL0 compiler

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_IDEN_LEN 11
#define MAX_NUM_LEN 5

// All of our reserved key words that the user cannot use as var names
char *reserved[14] = {"const", "var", "procedure", "call", "begin", "end", "if", "then", "else", "while", "do", "read", "write", "odd"};
// All of the reserved symbols
char symbols[13] = {'+', '-', '*', '/', '(', ')', '=', ',', '.', '<', '>', ';', ':'};
// All of the lexical tokens for PL0
char *enumString[33] = {"nulsym", "identsym", "numbersym", "plussym", "minussym",
                    "multsym", "slashsym", "oddsym", "eqsym", "neqsym",
                    "lessym", "leqsym", "gtrsym", "geqsym", "lparentsym",
                    "rparentsym", "commasym", "semicolonsym", "periodsym",
                    "becomessym", "beginsym", "endsym", "ifsym", "thensym",
                    "whilesym", "dosym", "callsym", "constsym", "varsym",
                    "procsym", "writesym", "readsym", "elsesym"
                  };

// Our enum for every token type.
typedef enum
{
	nulsym = 1, identsym, numbersym, plussym, minussym,
	multsym,  slashsym, oddsym, eqsym, neqsym, lessym, leqsym,
	gtrsym, geqsym, lparentsym, rparentsym, commasym, semicolonsym,
	periodsym, becomessym, beginsym, endsym, ifsym, thensym,
	whilesym, dosym, callsym, constsym, varsym, procsym, writesym,
	readsym , elsesym
} token_type;

// Our 'node' for our linked list.
typedef struct lex_token
{
    char s[15];
    token_type e;
    struct lex_token *next;
}lex_token;

// Creates a node for our LL.
lex_token *createNode(char *str)
{
    lex_token *node = malloc(sizeof(lex_token));

    strcpy(node->s, str);

    return node;
}

// Destroys the list.
lex_token *freeList(lex_token *head)
{
	if (head == NULL)
		return NULL;

	freeList(head->next);

	free(head);

	return NULL;
}

// Checks if s is contained in the reseved array.
// Returns index when found.
int isReserved(char *s)
{

    for (int i = 0; i < 14; i++)
    {
        if (strcmp(s, reserved[i]) == 0)
            return i;
    }

    return -1;
}

// Checks if c is contained in the symbols array.
// Returns index when found.
int isSymbol(char c)
{
	int res = -1;
	int letterRes = -1;
    for (int i = 0; i < 13; i++)
    {
        if (c == symbols[i])
            res = i;
    }

		if (isalpha(c) != 0 || c == 13 || isdigit(c) != 0)
		{
			res = 1;
		}


		return (res >= 0) ? 1 : 0;
}

// Head and tail field.
lex_token *head = NULL;
lex_token *tail = NULL;

// Insert function for LL
lex_token *insert(lex_token *head, char *str)
{
    int reserved = isReserved(str);

    if (head == NULL)
    {
        head = createNode(str);
        tail = head;

        return head;
    }

    tail->next = createNode(str);
    tail = tail->next;
    return head;
}

// Prints the LL recursively.
void print_list_recursive_helper(lex_token *head, FILE *op)
{
	if (head == NULL)
		return;

	fprintf(op, "%s\t%d\n", head->s, head->e);
	print_list_recursive_helper(head->next, op);
}

// Caller function for our recursive print
void printList(lex_token *head, FILE *op)
{
	if (head == NULL)
	{
		fprintf(op, "(empty list)\n");
		return;
	}
	print_list_recursive_helper(head, op);
	fprintf(op, "\n");
}

// This functions finds a node in the tokens list based on a string.
// Returns a token node.
lex_token *findNode(lex_token *head, char *str)
{
    lex_token *tmp = head;
    while (tmp != NULL)
    {

        if (strcmp(str, tmp->s) == 0)
            return tmp;

        tmp = tmp->next;
    }
    return NULL;
}

// Errors to detect:
// 1. Variable does not start with a letter.
//		-- if identifier starts with number then its wrong.
// 2. Number is too long.
//		-- if a number's length exceeds MAX_NUM_LEN
// 3. Name too long.
//		if a name's length exceeds MAX_IDEN_LEN
// 4. Invalid symbols.


// This function will print out the source code of the input file.
void printSourceProgram(FILE *op)
{
    FILE *fp = fopen("input.txt", "r");

    char c = fgetc(fp);

    if (fp == NULL)
    {
        return;
    }
    fprintf(op, "Source Program:\n");

    // Check if the character we got is at the end of the file and prints that char if not.
    while (c != EOF)
    {
        fprintf(op, "%c", c);
        c = fgetc(fp);
    }

    // Print a new line after all characters have been printed.
    fprintf(op, "\n\n");
    return;
}

// This functions returns 1 if the string is in bounds.
int isNumberInBounds(char *str)
{
	return (strlen(str) < MAX_NUM_LEN) ? 1 : 0;
}
// This functions returns 1 if the identifier is in bounds.
int isIdentInBounds(char *str)
{
	return (strlen(str) < MAX_IDEN_LEN) ? 1 : 0;
}

// Checks if the first character in the string is a letter/number.
int variableLetterCheck(char *str)
{
    return (isalpha(str[0]) != 0 ) ? 1 : 0;
}

// Fills str with null sentinals
char *resetString(char *str)
{
    int i = 0;
    for (i = strlen(str); i > 0; i--)
        str[i] = '\0';

    return str;
}

// Prints the lexcial table with a fancy recursive function.
void printLexemeTable(FILE *op)
{
    FILE *fp = fopen("input.txt", "r");

		fprintf(op, "Lexeme Table:\n");

    char line[200];
    char c;
    char *str = malloc(sizeof(char) * 100);
    char *idStr = malloc(sizeof(char) * 11);
		char *tmpStr;

	// Used for checking if a string is apart of a group
    int reserved = -1;
    int symbol = -1;

	// Main while loop
     while (!feof(fp))
     {
         int i;
         int l = 0;
		 int tempIndex = 0;

		// get a line of the input file, stored to the line string.
         fgets(line, 200, fp);

				 // Loops through the line, checks if the string is in a group
         for (i = 0; i < strlen(line); i++)
         {
					 // Ignore whitepace.
            if (line[i] == ' ' || line[i] == '\t' || line[i] == '\n')
                continue;

            str[l] = line[i];
            l++;

						// Checks if string is inside list.
						lex_token *isIdentifier = findNode(head, str);

						if (isIdentifier != NULL)
						{
							tmpStr = strstr(line, ":=");
							if (tmpStr != NULL)
							{
								if (tmpStr[0] == ':' && tmpStr[1] == '=')
								{
									head = insert(head, str);
									if (isIdentInBounds(str) == 1 && variableLetterCheck(str) == 1)
									{
										tail->e = identsym;
									}
									else
									{
										// Error, our number has a length that is too big.
										if (isIdentInBounds(str) != 1)
										{
											fprintf(op, "ERROR: Identifier exceeds max length of %d!\n", MAX_NUM_LEN);
										}

										printf("idStr: %s***\n", str);
										// Error, our identifier starts with a number.
										if (variableLetterCheck(idStr) != 1)
										{
											fprintf(op, "ERROR: Identifier starts with number\n");
										}
										exit(1);
									}

									head = insert(head, ":=");
									tail->e = becomessym;

									// loop until semicolon
									for (int j = 2; j < strlen(tmpStr); j++)
									{
										if (tmpStr[j] == ' ' || tmpStr[j] == '\t' || tmpStr[j] == '\n')
				                continue;

										switch (tmpStr[j])
										{
											case ';':
											head = insert(head, idStr);
											if (isNumberInBounds(idStr) == 1)
											{
												tail->e = numbersym;
											}
											else
											{
												// Error, our number has a length that is too big.
												fprintf(op, "ERROR: Number exceeds max length of %d!\n", MAX_NUM_LEN);
												exit(1);
											}

											idStr = resetString(idStr);
											tempIndex = 0;

											head = insert(head, ";");
											tail->e = semicolonsym;
											break;

											case '*':
											head = insert(head, idStr);
											if (isIdentInBounds(idStr) == 1 && variableLetterCheck(idStr) == 1)
											{
												tail->e = identsym;
											}
											else
											{
												// Error, our number has a length that is too big.
												if (isIdentInBounds(idStr) != 1)
												{
													fprintf(op, "ERROR: Identifier exceeds max length of %d!\n", MAX_NUM_LEN);
												}

												// Error, our identifier starts with a number.
												if (variableLetterCheck(idStr) != 1)
												{
													fprintf(op, "ERROR: Identifier starts with number\n");
												}
												exit(1);
											}

											idStr = resetString(idStr);
											tempIndex = 0;

											head = insert(head, "*");
											tail->e = multsym;
											break;

											case '+':
											// Inserting contents and resetting idStr
											head = insert(head, idStr);
											if (isIdentInBounds(idStr) == 1 && variableLetterCheck(idStr) == 1)
											{
												tail->e = identsym;
											}
											else
											{
												// Error, our number has a length that is too big.
												if (isIdentInBounds(idStr) != 1)
												{
													fprintf(op, "ERROR: Identifier exceeds max length of %d!\n", MAX_NUM_LEN);
												}

												// Error, our identifier starts with a number.
												if (variableLetterCheck(idStr) != 1)
												{
													fprintf(op, "ERROR: Identifier starts with number\n");
												}
												exit(1);
											}

											idStr = resetString(idStr);
											tempIndex = 0;

											head = insert(head, "+");
											tail->e = plussym;

											break;

											case '-':
											head = insert(head, idStr);
											if (isIdentInBounds(idStr) == 1 && variableLetterCheck(idStr) == 1)
											{
												tail->e = identsym;
											}
											else
											{
												// Error, our number has a length that is too big.
												if (isIdentInBounds(idStr) != 1)
												{
													fprintf(op, "ERROR: Identifier exceeds max length of %d!\n", MAX_NUM_LEN);
												}

												// Error, our identifier starts with a number.
												if (variableLetterCheck(idStr) != 1)
												{
													fprintf(op, "ERROR: Identifier starts with number\n");
												}
												exit(1);
											}

											idStr = resetString(idStr);
											tempIndex = 0;

											head = insert(head, "-");
											tail->e = minussym;
											break;

											case '/':
											head = insert(head, idStr);
											if (isIdentInBounds(idStr) == 1 && variableLetterCheck(idStr) == 1)
											{
												tail->e = identsym;
											}
											else
											{
												// Error, our number has a length that is too big.
												if (isIdentInBounds(idStr) != 1)
												{
													fprintf(op, "ERROR: Identifier exceeds max length of %d!\n", MAX_NUM_LEN);
												}

												// Error, our identifier starts with a number.
												if (variableLetterCheck(idStr) != 1)
												{
													fprintf(op, "ERROR: Identifier starts with number\n");
												}
												exit(1);
											}

											idStr = resetString(idStr);
											tempIndex = 0;

											head = insert(head, "/");
											tail->e = slashsym;
											break;

											default:
											idStr[tempIndex] = tmpStr[j];
											tempIndex++;
											break;
										}
									}
								}
							}
						}
						int reserved = isReserved(str);

            // Checks if the string is a reserved word.
            if (reserved != -1)
            {
								// Puts reserved word inside list.
                head = insert(head, str);

								// Resets string (l is used for str's index inside the loop.)
                str = resetString(str);
                l = 0;

								// Which reserved word is it?
                switch (reserved)
                {
										// constant word
                    case 0:
                    tail->e = constsym;

										int j;
										int k = 0;

										for (j = i + 1; j < strlen(line); j++)
                    {
                        if (line[j] == ' ' || line[j] == '\t' || line[j] == '\n')
                            continue;

                        // Cases depening on what char we get from the j loop.
                        switch(line[j])
                        {
														case '=':
														head = insert(head, idStr);
														if (isIdentInBounds(idStr) == 1 && variableLetterCheck(idStr) == 1)
														{
															tail->e = identsym;
														}
														else
														{
															// Error, our number has a length that is too big.
															if (isIdentInBounds(idStr) != 1)
															{
																fprintf(op, "ERROR: Identifier exceeds max length of %d!\n", MAX_NUM_LEN);
															}

															// Error, our identifier starts with a number.
															if (variableLetterCheck(idStr) != 1)
															{
																fprintf(op, "ERROR: Identifier starts with number\n");
															}
															exit(1);
														}
														idStr = resetString(idStr);
														k = 0;

														head = insert(head, "=");
														tail->e = eqsym;
														break;

                            // If we have a semicolon, store our idStr as a token.
                            case ';':
                            head = insert(head, idStr);
														if (isNumberInBounds(idStr) == 1)
														{
															tail->e = numbersym;
														}
														else
														{
															// Error, our number has a length that is too big.
															fprintf(op, "ERROR: Number exceeds max length of %d!\n", MAX_NUM_LEN);
															exit(1);
														}

                            idStr = resetString(idStr);
                            k = 0;

                            head = insert(head, ";");
                            tail->e = semicolonsym;
                            break;

                            // If we have a comma, we must find the other identifier.
                            case ',':
														if (strlen(idStr) > MAX_NUM_LEN)
														{
															printf("Error\n" );
															break;
														}
														else
														{
															head = insert(head, idStr);
															if (isNumberInBounds(idStr) == 1)
															{
																tail->e = numbersym;
															}
															else
															{
																// Error, our number has a length that is too big.
																fprintf(op, "ERROR: Number exceeds max length of %d!\n", MAX_NUM_LEN);
																exit(1);
															}

	                            idStr = resetString(idStr);
	                            k = 0;

	                            head = insert(head, ",");
	                            tail->e = commasym;
	                            break;
														}
                            default:
                            // Filling new string (if we get any other char...)
                            idStr[k] = line[j];
                            k++;
                            break;
                        }
                    }
                    break;

										// var word.
                    case 1:
										k = 0;
                    tail->e = varsym;
                    // We need to get the identifier(s) and/or commas/smemicolon.


                    for (j = i + 1; j < strlen(line); j++)
                    {
                        if (line[j] == ' ' || line[j] == '\t' || line[j] == '\n')
                            continue;

                        // Cases depening on what char we get from the j loop.
                        switch(line[j])
                        {
                            // If we have a semicolon, store our idStr as a token.
                            case ';':
                            head = insert(head, idStr);
														if (isIdentInBounds(idStr) == 1 && variableLetterCheck(idStr) == 1)
														{
															tail->e = identsym;
														}
														else
														{
															// Error, our number has a length that is too big.
															if (isIdentInBounds(idStr) != 1)
															{
																fprintf(op, "ERROR: Identifier exceeds max length of %d!\n", MAX_NUM_LEN);
															}

															// Error, our identifier starts with a number.
															if (variableLetterCheck(idStr) != 1)
															{
																fprintf(op, "ERROR: Identifier starts with number\n");
															}
															exit(1);
														}

                            idStr = resetString(idStr);
                            k = 0;

                            head = insert(head, ";");
                            tail->e = semicolonsym;
                            break;

                            // If we have a comma, we must find the other identifier.
                            case ',':
                            // Insert what we have in our idStr & reset it.
                            head = insert(head, idStr);
														if (isIdentInBounds(idStr) == 1 && variableLetterCheck(idStr) == 1)
														{
															tail->e = identsym;
														}
														else
														{
															// Error, our number has a length that is too big.
															if (isIdentInBounds(idStr) != 1)
															{
																fprintf(op, "ERROR: Identifier exceeds max length of %d!\n", MAX_NUM_LEN);
															}

															// Error, our identifier starts with a number.
															if (variableLetterCheck(idStr) != 1)
															{
																fprintf(op, "ERROR: Identifier starts with number\n");
															}
															exit(1);
														}

                            idStr = resetString(idStr);
                            k = 0;

                            head = insert(head, ",");
                            tail->e = commasym;
                            break;

                            default:
                            // Filling new string (if we get any other char...)
                            idStr[k] = line[j];
                            k++;
                            break;
                        }
                    }

                    break;

										// procedure word
                    case 2:
										 k = 0;
                     tail->e = procsym;
                    break;

										// call word.
                    case 3:
                    tail->e = callsym;
										// We need to get the identifier(s) and/or commas/smemicolon.
										k = 0;

										for (j = i + 1; j < strlen(line); j++)
										{
												if (line[j] == ' ' || line[j] == '\t' || line[j] == '\n')
														continue;

												// Cases depening on what char we get from the j loop.
												switch(line[j])
												{
														// If we have a semicolon, store our idStr as a token.
														case ';':
														head = insert(head, idStr);
														if (isIdentInBounds(idStr) == 1 && variableLetterCheck(idStr) == 1)
														{
															tail->e = identsym;
														}
														else
														{
															// Error, our number has a length that is too big.
															if (isIdentInBounds(idStr) != 1)
															{
																fprintf(op, "ERROR: Identifier exceeds max length of %d!\n", MAX_NUM_LEN);
															}

															// Error, our identifier starts with a number.
															if (variableLetterCheck(idStr) != 1)
															{
																fprintf(op, "ERROR: Identifier starts with number\n");
															}
															exit(1);
														}

														idStr = resetString(idStr);
														k = 0;

														head = insert(head, ";");
														tail->e = semicolonsym;
														break;

														// ASCII 13 is the carrige return (not picked up by our loop)
														// This if statement checks if there isn't a semicolon
														// at the end of the line.
														case 13:
														if (strstr(line, ";") == NULL)
														{
															head = insert(head, idStr);
															if (isIdentInBounds(idStr) == 1 && variableLetterCheck(idStr) == 1)
															{
																tail->e = identsym;
															}
															else
															{
																// Error, our number has a length that is too big.
																if (isIdentInBounds(idStr) != 1)
																{
																	fprintf(op, "ERROR: Identifier exceeds max length of %d!\n", MAX_NUM_LEN);
																}

																// Error, our identifier starts with a number.
																if (variableLetterCheck(idStr) != 1)
																{
																	fprintf(op, "ERROR: Identifier starts with number\n");
																}
																exit(1);
															}

															idStr = resetString(idStr);
															k = 0;
														}

														break;

														default:
														// Filling new string (if we get any other char...)
														idStr[k] = line[j];
														k++;

														break;
												}
										}
                    break;

										// begin word.
                    case 4:
										k = 0;
                    tail->e = beginsym;
                    str = resetString(str);
                    l = 0;
                    break;

										// end word.
                    case 5:
										k = 0;
                    tail->e = endsym;
										// if theres a period then add it :)
										if (strstr(line, ".") != NULL)
										{
											head = insert(head, ".");
											tail->e = periodsym;
										}
                    break;

										// if word
                    case 6:
										k = 0;
                    tail->e = ifsym;
										// Check condition.
										// We need to get the identifier(s) and/or commas/smemicolon.
										k = 0;

										for (j = i + 1; j < strlen(line); j++)
										{
												if (line[j] == ' ' || line[j] == '\t' || line[j] == '\n')
														continue;

												// Cases depening on what char we get from the j loop.
												switch(line[j])
												{

														// ASCII 13 is the carrige return (not picked up by our loop)
														// This if statement checks if there isn't a semicolon
														// at the end of the line.
														case 'o':
														if ((j + 2) < strlen(line) && line[j + 1] == 'd' && line[j + 2] == 'd')
														{
															head = insert(head, "odd");
															tail->e = oddsym;
															j += 2;

															idStr = resetString(idStr);
															k = 0;
														}
														break;

														case 13:
														if (strstr(line, ";") == NULL)
														{
															// If then is present, go back 4 spaces in idStr.

															if (strstr(line, "then") != NULL)
															{

																// Removing odd from the string
																int tempValue = strlen(idStr) - 1;
																idStr[tempValue] = '\0';
																idStr[tempValue - 1] = '\0';
																idStr[tempValue - 2] = '\0';
																idStr[tempValue - 3] = '\0';

																head = insert(head, idStr);

																if (isdigit(idStr[0]) == 0)
																{
																	if (isIdentInBounds(idStr) == 1 && variableLetterCheck(idStr) == 1)
																	{
																		tail->e = identsym;
																	}
																	else
																	{
																		// Error, our number has a length that is too big.
																		if (isIdentInBounds(idStr) != 1)
																		{
																			fprintf(op, "ERROR: Identifier exceeds max length of %d!\n", MAX_NUM_LEN);
																		}

																		// Error, our identifier starts with a number.
																		if (variableLetterCheck(idStr) != 1)
																		{
																			fprintf(op, "ERROR: Identifier starts with number\n");
																		}
																		exit(1);
																	}
																}
																else
																{
																	if (isNumberInBounds(idStr) == 1)
																	{
																		tail->e = numbersym;
																	}
																	else
																	{
																		// Error, our number has a length that is too big.
																		fprintf(op, "ERROR: Number exceeds max length of %d!\n", MAX_NUM_LEN);
																		exit(1);
																	}
																}
																idStr = resetString(idStr);
																k = 0;

																head = insert(head, "then");
																tail->e = thensym;

																break;
															}

															head = insert(head, idStr);
															if (isdigit(idStr[0]) == 0)
															{
																if (isIdentInBounds(idStr) == 1 && variableLetterCheck(idStr) == 1)
																{
																	tail->e = identsym;
																}
																else
																{
																	// Error, our number has a length that is too big.
																	if (isIdentInBounds(idStr) != 1)
																	{
																		fprintf(op, "ERROR: Identifier exceeds max length of %d!\n", MAX_NUM_LEN);
																	}

																	// Error, our identifier starts with a number.
																	if (variableLetterCheck(idStr) != 1)
																	{
																		fprintf(op, "ERROR: Identifier starts with number\n");
																	}
																	exit(1);
																}
															}
															else
															{
																if (isNumberInBounds(idStr) == 1)
																{
																	tail->e = numbersym;
																}
																else
																{
																	// Error, our number has a length that is too big.
																	fprintf(op, "ERROR: Number exceeds max length of %d!\n", MAX_NUM_LEN);
																	exit(1);
																}
															}

															printf("\t> strlen(idStr): %ld\n", strlen(idStr));
															printf("\t> idStr: %s\n", idStr);

															idStr = resetString(idStr);
															k = 0;
														}

														break;

														case '=':
														head = insert(head, idStr);

														if (isdigit(idStr[0]) == 0)
														{
															if (isIdentInBounds(idStr) == 1 && variableLetterCheck(idStr) == 1)
															{
																tail->e = identsym;
															}
															else
															{
																// Error, our number has a length that is too big.
																if (isIdentInBounds(idStr) != 1)
																{
																	fprintf(op, "ERROR: Identifier exceeds max length of %d!\n", MAX_NUM_LEN);
																}

																// Error, our identifier starts with a number.
																if (variableLetterCheck(idStr) != 1)
																{
																	fprintf(op, "ERROR: Identifier starts with number\n");
																}
																exit(1);
															}
														}
														else
														{
															if (isNumberInBounds(idStr) == 1)
															{
																tail->e = numbersym;
															}
															else
															{
																// Error, our number has a length that is too big.
																fprintf(op, "ERROR: Number exceeds max length of %d!\n", MAX_NUM_LEN);
																exit(1);
															}
														}

														head = insert(head, "=");
														tail->e = eqsym;

														idStr = resetString(idStr);
														k = 0;

														break;

														case '>':
														head = insert(head, idStr);

														if (isdigit(idStr[0]) == 0)
														{
															if (isIdentInBounds(idStr) == 1 && variableLetterCheck(idStr) == 1)
															{
																tail->e = identsym;
															}
															else
															{
																// Error, our number has a length that is too big.
																if (isIdentInBounds(idStr) != 1)
																{
																	fprintf(op, "ERROR: Identifier exceeds max length of %d!\n", MAX_NUM_LEN);
																}

																// Error, our identifier starts with a number.
																if (variableLetterCheck(idStr) != 1)
																{
																	fprintf(op, "ERROR: Identifier starts with number\n");
																}
																exit(1);
															}
														}
														else
														{
															if (isNumberInBounds(idStr) == 1)
															{
																tail->e = numbersym;
															}
															else
															{
																// Error, our number has a length that is too big.
																fprintf(op, "ERROR: Number exceeds max length of %d!\n", MAX_NUM_LEN);
																exit(1);
															}
														}

														if ((j + 1) < strlen(line))
														{
															if (line[j + 1] == '=')
															{
																head = insert(head, ">=");
																tail->e = geqsym;

																idStr = resetString(idStr);
																k = 0;
																j++;

																break;
															}
														}


														head = insert(head, ">");
														tail->e = gtrsym;

														idStr = resetString(idStr);
														k = 0;

														break;

														case '<':
														head = insert(head, idStr);

														if (isdigit(idStr[0]) == 0)
														{
															if (isIdentInBounds(idStr) == 1 && variableLetterCheck(idStr) == 1)
															{
																tail->e = identsym;
															}
															else
															{
																// Error, our number has a length that is too big.
																if (isIdentInBounds(idStr) != 1)
																{
																	fprintf(op, "ERROR: Identifier exceeds max length of %d!\n", MAX_NUM_LEN);
																}

																// Error, our identifier starts with a number.
																if (variableLetterCheck(idStr) != 1)
																{
																	fprintf(op, "ERROR: Identifier starts with number\n");
																}
																exit(1);
															}
														}
														else
														{
															if (isNumberInBounds(idStr) == 1)
															{
																tail->e = numbersym;
															}
															else
															{
																// Error, our number has a length that is too big.
																fprintf(op, "ERROR: Number exceeds max length of %d!\n", MAX_NUM_LEN);
																exit(1);
															}
														}

														if ((j + 1) < strlen(line))
														{
															if (line[j + 1] == '>')
															{
																head = insert(head, "<>");
																tail->e = neqsym;

																idStr = resetString(idStr);
																k = 0;
																j++;
																break;
															}

															if (line[j + 1] == '=')
															{
																head = insert(head, "<=");
																tail->e = leqsym;

																idStr = resetString(idStr);
																k = 0;
																j++;
																break;
															}
														}


														head = insert(head, "<");
														tail->e = lessym;

														idStr = resetString(idStr);
														k = 0;

														break;

														default:
														// Filling new string (if we get any other char...)
														idStr[k] = line[j];
														k++;
														break;
												}
										}
                    break;

					          // then word
                    case 7:
                    tail->e = thensym;
                    break;

					          // else word
                    case 8:
                    tail->e = elsesym;
                    break;

					// while word
					case 9:
                    tail->e = whilesym;
										// Check condition.
										// We need to get the identifier(s) and/or commas/smemicolon.
										k = 0;

										for (j = i + 1; j < strlen(line); j++)
										{
												if (line[j] == ' ' || line[j] == '\t' || line[j] == '\n')
														continue;

												// Cases depening on what char we get from the j loop.
												switch(line[j])
												{

														// ASCII 13 is the carrige return (not picked up by our loop)
														// This if statement checks if there isn't a semicolon
														// at the end of the line.
														case 'o':
														if ((j + 2) < strlen(line) && line[j + 1] == 'd' && line[j + 2] == 'd')
														{
															head = insert(head, "odd");
															tail->e = oddsym;
															j += 2;

															idStr = resetString(idStr);
															k = 0;
														}
														break;

														case 13:
														if (strstr(line, ";") == NULL)
														{
															// If then is present, go back 4 spaces in idStr.

															if (strstr(line, "do") != NULL)
															{

																// Removing odd from the string
																int tempValue = strlen(idStr) - 1;
																idStr[tempValue] = '\0';

																head = insert(head, idStr);

																if (isdigit(idStr[0]) == 0)
																{
																	if (isIdentInBounds(idStr) == 1 && variableLetterCheck(idStr) == 1)
																	{
																		tail->e = identsym;
																	}
																	else
																	{
																		// Error, our number has a length that is too big.
																		if (isIdentInBounds(idStr) != 1)
																		{
																			fprintf(op, "ERROR: Identifier exceeds max length of %d!\n", MAX_NUM_LEN);
																		}

																		// Error, our identifier starts with a number.
																		if (variableLetterCheck(idStr) != 1)
																		{
																			fprintf(op, "ERROR: Identifier starts with number\n");
																		}
																		exit(1);
																	}
																}
																else
																{
																	if (isNumberInBounds(idStr) == 1)
																	{
																		tail->e = numbersym;
																	}
																	else
																	{
																		// Error, our number has a length that is too big.
																		fprintf(op, "ERROR: Number exceeds max length of %d!\n", MAX_NUM_LEN);
																		exit(1);
																	}
																}
																idStr = resetString(idStr);
																k = 0;

																head = insert(head, "do");
																tail->e = dosym;

																break;
															}

															head = insert(head, idStr);
															if (isdigit(idStr[0]) == 0)
															{
																if (isIdentInBounds(idStr) == 1 && variableLetterCheck(idStr) == 1)
																{
																	tail->e = identsym;
																}
																else
																{
																	// Error, our number has a length that is too big.
																	if (isIdentInBounds(idStr) != 1)
																	{
																		fprintf(op, "ERROR: Identifier exceeds max length of %d!\n", MAX_NUM_LEN);
																	}

																	// Error, our identifier starts with a number.
																	if (variableLetterCheck(idStr) != 1)
																	{
																		fprintf(op, "ERROR: Identifier starts with number\n");
																	}
																	exit(1);
																}
															}
															else
															{
																if (isNumberInBounds(idStr) == 1)
																{
																	tail->e = numbersym;
																}
																else
																{
																	// Error, our number has a length that is too big.
																	fprintf(op, "ERROR: Number exceeds max length of %d!\n", MAX_NUM_LEN);
																	exit(1);
																}
															}

															printf("\t> strlen(idStr): %ld\n", strlen(idStr));
															printf("\t> idStr: %s\n", idStr);

															idStr = resetString(idStr);
															k = 0;
														}

														break;

														case '=':
														head = insert(head, idStr);

														if (isdigit(idStr[0]) == 0)
														{
															if (isIdentInBounds(idStr) == 1 && variableLetterCheck(idStr) == 1)
															{
																tail->e = identsym;
															}
															else
															{
																// Error, our number has a length that is too big.
																if (isIdentInBounds(idStr) != 1)
																{
																	fprintf(op, "ERROR: Identifier exceeds max length of %d!\n", MAX_NUM_LEN);
																}

																// Error, our identifier starts with a number.
																if (variableLetterCheck(idStr) != 1)
																{
																	fprintf(op, "ERROR: Identifier starts with number\n");
																}
																exit(1);
															}
														}
														else
														{
															if (isNumberInBounds(idStr) == 1)
															{
																tail->e = numbersym;
															}
															else
															{
																// Error, our number has a length that is too big.
																fprintf(op, "ERROR: Number exceeds max length of %d!\n", MAX_NUM_LEN);
																exit(1);
															}
														}

														head = insert(head, "=");
														tail->e = eqsym;

														idStr = resetString(idStr);
														k = 0;

														break;

														case '>':
														head = insert(head, idStr);

														if (isdigit(idStr[0]) == 0)
														{
															if (isIdentInBounds(idStr) == 1 && variableLetterCheck(idStr) == 1)
															{
																tail->e = identsym;
															}
															else
															{
																// Error, our number has a length that is too big.
																if (isIdentInBounds(idStr) != 1)
																{
																	fprintf(op, "ERROR: Identifier exceeds max length of %d!\n", MAX_NUM_LEN);
																}

																// Error, our identifier starts with a number.
																if (variableLetterCheck(idStr) != 1)
																{
																	fprintf(op, "ERROR: Identifier starts with number\n");
																}
																exit(1);
															}
														}
														else
														{
															if (isNumberInBounds(idStr) == 1)
															{
																tail->e = numbersym;
															}
															else
															{
																// Error, our number has a length that is too big.
																fprintf(op, "ERROR: Number exceeds max length of %d!\n", MAX_NUM_LEN);
																exit(1);
															}
														}

														if ((j + 1) < strlen(line))
														{
															if (line[j + 1] == '=')
															{
																head = insert(head, ">=");
																tail->e = geqsym;

																idStr = resetString(idStr);
																k = 0;
																j++;

																break;
															}
														}


														head = insert(head, ">");
														tail->e = gtrsym;

														idStr = resetString(idStr);
														k = 0;

														break;

														case '<':
														head = insert(head, idStr);

														if (isdigit(idStr[0]) == 0)
														{
															if (isIdentInBounds(idStr) == 1 && variableLetterCheck(idStr) == 1)
															{
																tail->e = identsym;
															}
															else
															{
																// Error, our number has a length that is too big.
																if (isIdentInBounds(idStr) != 1)
																{
																	fprintf(op, "ERROR: Identifier exceeds max length of %d!\n", MAX_NUM_LEN);
																}

																// Error, our identifier starts with a number.
																if (variableLetterCheck(idStr) != 1)
																{
																	fprintf(op, "ERROR: Identifier starts with number\n");
																}
																exit(1);
															}
														}
														else
														{
															if (isNumberInBounds(idStr) == 1)
															{
																tail->e = numbersym;
															}
															else
															{
																// Error, our number has a length that is too big.
																fprintf(op, "ERROR: Number exceeds max length of %d!\n", MAX_NUM_LEN);
																exit(1);
															}
														}

														if ((j + 1) < strlen(line))
														{
															if (line[j + 1] == '>')
															{
																head = insert(head, "<>");
																tail->e = neqsym;

																idStr = resetString(idStr);
																k = 0;
																j++;
																break;
															}

															if (line[j + 1] == '=')
															{
																head = insert(head, "<=");
																tail->e = leqsym;

																idStr = resetString(idStr);
																k = 0;
																j++;
																break;
															}
														}


														head = insert(head, "<");
														tail->e = lessym;

														idStr = resetString(idStr);
														k = 0;

														break;

														default:
														// Filling new string (if we get any other char...)
														idStr[k] = line[j];
														k++;
														break;
												}
										}
                    break;

										// do word
                    case 10:
                    tail->e = dosym;
                    break;

										// read word
                    case 11:
                    tail->e = readsym;
										// We need to get the identifier(s) and/or commas/smemicolon.
										k = 0;

										for (j = i + 1; j < strlen(line); j++)
										{
												if (line[j] == ' ' || line[j] == '\t' || line[j] == '\n')
														continue;

												// Cases depening on what char we get from the j loop.
												switch(line[j])
												{
														// If we have a semicolon, store our idStr as a token.
														case ';':
														head = insert(head, idStr);
														if (isIdentInBounds(idStr) == 1 && variableLetterCheck(idStr) == 1)
														{
															tail->e = identsym;
														}
														else
														{
															// Error, our number has a length that is too big.
															if (isIdentInBounds(idStr) != 1)
															{
																fprintf(op, "ERROR: Identifier exceeds max length of %d!\n", MAX_NUM_LEN);
															}

															// Error, our identifier starts with a number.
															if (variableLetterCheck(idStr) != 1)
															{
																fprintf(op, "ERROR: Identifier starts with number\n");
															}
															exit(1);
														};

														idStr = resetString(idStr);
														k = 0;

														head = insert(head, ";");
														tail->e = semicolonsym;
														break;

														// ASCII 13 is the carrige return (not picked up by our loop)
														// This if statement checks if there isn't a semicolon
														// at the end of the line.
														case 13:
														if (strstr(line, ";") == NULL)
														{
															head = insert(head, idStr);
															if (isIdentInBounds(idStr) == 1 && variableLetterCheck(idStr) == 1)
															{
																tail->e = identsym;
															}
															else
															{
																// Error, our number has a length that is too big.
																if (isIdentInBounds(idStr) != 1)
																{
																	fprintf(op, "ERROR: Identifier exceeds max length of %d!\n", MAX_NUM_LEN);
																}

																// Error, our identifier starts with a number.
																if (variableLetterCheck(idStr) != 1)
																{
																	fprintf(op, "ERROR: Identifier starts with number\n");
																}
																exit(1);
															}

															idStr = resetString(idStr);
															k = 0;
														}

														break;

														default:
														// Filling new string (if we get any other char...)
														idStr[k] = line[j];
														k++;

														break;
												}
										}
                    break;

										// write word
                    case 12:
                    tail->e = writesym;
										// We need to get the identifier(s) and/or commas/smemicolon.
										k = 0;

										for (j = i + 1; j < strlen(line); j++)
										{
												if (line[j] == ' ' || line[j] == '\t' || line[j] == '\n')
														continue;

												// Cases depening on what char we get from the j loop.
												switch(line[j])
												{
														// If we have a semicolon, store our idStr as a token.
														case ';':
														head = insert(head, idStr);
														if (isIdentInBounds(idStr) == 1 && variableLetterCheck(idStr) == 1)
														{
															tail->e = identsym;
														}
														else
														{
															// Error, our number has a length that is too big.
															if (isIdentInBounds(idStr) != 1)
															{
																fprintf(op, "ERROR: Identifier exceeds max length of %d!\n", MAX_NUM_LEN);
															}

															// Error, our identifier starts with a number.
															if (variableLetterCheck(idStr) != 1)
															{
																fprintf(op, "ERROR: Identifier starts with number\n");
															}
															exit(1);
														}

														idStr = resetString(idStr);
														k = 0;

														head = insert(head, ";");
														tail->e = semicolonsym;
														break;

														// ASCII 13 is the carrige return (not picked up by our loop)
														// This if statement checks if there isn't a semicolon
														// at the end of the line.
														case 13:
														if (strstr(line, ";") == NULL)
														{
															head = insert(head, idStr);
															if (isIdentInBounds(idStr) == 1 && variableLetterCheck(idStr) == 1)
															{
																tail->e = identsym;
															}
															else
															{
																// Error, our number has a length that is too big.
																if (isIdentInBounds(idStr) != 1)
																{
																	fprintf(op, "ERROR: Identifier exceeds max length of %d!\n", MAX_NUM_LEN);
																}

																// Error, our identifier starts with a number.
																if (variableLetterCheck(idStr) != 1)
																{
																	fprintf(op, "ERROR: Identifier starts with number\n");
																}
																exit(1);
															}

															idStr = resetString(idStr);
															k = 0;
														}

														break;

														default:
														// Filling new string (if we get any other char...)
														idStr[k] = line[j];
														k++;

														break;
												}
										}
                    break;

										// odd word.
                    case 13:
                    tail->e = oddsym;
                    break;

                }
            }

						// Checks if its a symbol, if so, symbol != -1.
            if (isSymbol(line[i]) == 0)
						{
							fprintf(op, "ERROR: Invalid symbol. (%c)\n", line[i]);
							exit(1);
						}

         }
         // Reset the string after every iteration.
         str = resetString(str);
         l = 0;
    }

    printList(head, op);

		// Does necessary operations to free up memory, woo!
    fclose(fp);
		free(idStr);
		free(str);

    return;
}

// Aformentioned fancy printing funtion.
void printLexemeListRecursive(lex_token *head, FILE *op)
{
    if (head == NULL)
        return;


    if (head->e != identsym)
    {
        fprintf(op, "%d", head->e);
    }
    else
    {
        fprintf(op, "%d %s", head->e, head->s);
    }

    if (head->e == numbersym)
    {
        fprintf(op, " %s", head->s);
    }

    if (head->next !=NULL)
        {
            fprintf(op, " ");
        }
        else
        {
            fprintf(op, "\n");
        }

        printLexemeListRecursive(head->next, op);
        return;
}

// Prints the lexeme list with the help of recursion!
void printLexemeList(FILE *op)
{
    printLexemeListRecursive(head, op);

    return;
}
