#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/* Array of structs to hold the C variables, each struct has a char and an int. 
    The char is the variable name and the int is the value*/
typedef struct {
    char variable;
    int value;
} CharIntPair;

void printCharIntPairs(CharIntPair c_variables[8]) {
    for (int i = 0; i < 8; i++) {
        if (c_variables[i].variable != ' ') { // Check if the struct is initialized
            printf("Index %d: Char = %c, Int = %d\n", i, c_variables[i].variable, c_variables[i].value);
        }
    }
}

// Function to check which saved register is free and returns the index of the free register
int checkFree(CharIntPair c_variables[], int size) {
    for (int i = 0; i < size; i++) {
        if (c_variables[i].variable == ' ') {
            return i;
        }
    }
    return -1;
}

int findIndex(CharIntPair c_variables[], int size, char target) {
    for (int i = 0; i < size; i++) {
        if (c_variables[i].variable == target) {
            return i;
        }
    }
    return -1;
}

// Function to declare a variable and assign it a value in the saved registers
void declareVariable(CharIntPair c_variables[8], char variable, int value) {
    // Loop through the array of structs and check if the variable has been declared
    int index = checkFree(c_variables, 8);
    if (index == -1) {
        printf("Error: No free registers\n");
        exit(1);
    }
    c_variables[index].variable = variable;
    c_variables[index].value = value;
    printf("li $s%d, %d\n", index, value);
}

void appendToCurrEquation(char **curr_equation, char character) {
    // Find the current length of the curr_equation
    size_t curr_length = *curr_equation ? strlen(*curr_equation) : 0;
    
    // Allocate memory for the new curr_equation
    char *new_equation = (char *)malloc(curr_length + 2); // +2 for the new character and null terminator

    if (!new_equation) {
        // Handle memory allocation error
        fprintf(stderr, "Memory allocation failed.");
        exit(1);
    }

    // Copy the existing curr_equation, if it exists
    if (*curr_equation) {
        strcpy(new_equation, *curr_equation);
    }

    // Append the new character to curr_equation
    new_equation[curr_length] = character;
    new_equation[curr_length + 1] = '\0';

    // Update curr_equation pointer to the new string
    *curr_equation = new_equation;
}

void translateAddTemp(char *curr_equation, CharIntPair c_variables[8], CharIntPair temporary_registers[10], int temp_Register) {
    char delim = '+'; // Delimiter to split the string
    char *token = strtok(curr_equation, &delim);
    
    char *elements[2]; 

    int index = 0;
    while (token != NULL) {
        elements[index] = token;
        index++;
        token = strtok(NULL, &delim);
    }

    int value = 0;

    if (isalpha(*elements[0]) != 0) {
        if (isalpha(*elements[1]) != 0) {
            int index1 = findIndex(c_variables, 8, *elements[0]);
            int index2 = findIndex(c_variables, 8, *elements[1]);
            value = c_variables[index1].value + c_variables[index2].value;
            printf("add $t%d, $s%d, $s%d\n", temp_Register, index1, index2);
        }
        else if (isdigit(*elements[1]) != 0) {
            int index1 = findIndex(c_variables, 8, *elements[0]);
            value = c_variables[index1].value + atoi(elements[1]);
            printf("addi $t%d, $s%d, %d\n", temp_Register, index1, atoi(elements[1]));
        }
    }
    else if (isdigit(*elements[0]) != 0) {
        if (isalpha(*elements[1]) != 0) {
            int index2 = findIndex(c_variables, 8, *elements[1]);
            value = c_variables[index2].value + atoi(elements[0]);
            printf("addi $t%d, $s%d, %d\n", temp_Register, index2, atoi(elements[0]));
        }
        else if (isdigit(*elements[1]) != 0) {
            value = atoi(elements[0]) + atoi(elements[1]);
            printf("addi $t%d, $s%d, %d\n", temp_Register, atoi(elements[0]), atoi(elements[1]));
        }
    }
    temporary_registers[temp_Register].variable = 't';
    temporary_registers[temp_Register].value = value;
}

void translatetoMIPS(char *file_string, CharIntPair c_variables[8]) {
    // Intialize an array of structs to hold the temporary registers
    CharIntPair temporary_registers[10];
    for (int i = 0; i < 8; i++) {
        temporary_registers[i].variable = ' ';
        temporary_registers[i].value = 0;
    }
    char *newline = strchr(file_string, '\n');
    if (newline != NULL) {
        *newline = '\0';
    }
    printf("# %s\n", file_string);

    int i = 0;
    // Removing all spaces from the string to make it easier to parse
    while (file_string[i] != '\0') {
        if (file_string[i] == ' ') {
            for (int j = i; file_string[j] != '\0'; j++) {
                file_string[j] = file_string[j + 1];
            }
        }
        i++;
    }

    // The first letter should always be a letter, and the second should always be an equals sign
    if (isalpha(file_string[0]) != 0 && file_string[1] == '=') {
        // If the third character is a number, then it is a VARIABLE DECLARATION. 
        if (isdigit(file_string[2]) != 0 || file_string[2] == '-') {
            int index = 2;
            int result = 0;
            int multiplier = 1;
            // If it is negative, then the multiplier is -1 to make the result negative
            if (file_string[2] == '-') {
                index++;
                multiplier = -1;
            }
            while (file_string[index] != ';') {
                result = result * 10 + (file_string[index] - '0');
                index++;
            }
            result = result * multiplier;
            declareVariable(c_variables, file_string[0], result);
        }
        // Else loop through the string and translate the expression
        else {
            int index = 2;
            char *curr_equation = NULL;
            bool isNegative = false;
            while (file_string[index] != ';') {
                // If the character is a letter, then it is a variable
                if (isalpha(file_string[index]) != 0) {
                    // Check the variable is in the saved registers
                    if (findIndex(c_variables, 8, file_string[index]) != -1) {
                        // If the equation is empty, then it is the first variable in the equation (lhs of the operator). If not, then if the last char is a operator, then append the variable (rhs of the operator)
                        if (curr_equation == NULL || (curr_equation != NULL && (curr_equation[strlen(curr_equation) - 1] == '+' || curr_equation[strlen(curr_equation) - 1] == '-' 
                            || curr_equation[strlen(curr_equation) - 1] == '*' || curr_equation[strlen(curr_equation) - 1] == '/' || curr_equation[strlen(curr_equation) - 1] == '%'))) {
                            appendToCurrEquation(&curr_equation, file_string[index]);
                        }
                    }
                }
                // If the character is an operand
                else if (file_string[index] == '+' || file_string[index] == '-' || file_string[index] == '*' || file_string[index] == '/' || file_string[index] == '%') {
                    // Check if the equation is not empty, and if the last char is a digit or a letter. If yes, then append the operand to the equation
                    int hasOtherOperands = 0;
                    for (size_t i = 0; i < strlen(curr_equation); i++) {
                        if (curr_equation[i] == '+' || curr_equation[i] == '-' || curr_equation[i] == '*' || curr_equation[i] == '/' || curr_equation[i] == '%') {
                            hasOtherOperands = 1; 
                            break;
                        }
                    }
                    // If the curr is a negative sign, then check if the previous char is a minus operand. If yes, then the current is the dash for the negative number
                    if ((file_string[index-1] == '+' || file_string[index-1] == '-' || file_string[index-1] == '*' || file_string[index-1] == '/' || file_string[index-1] == '%') && file_string[index] == '-' && isdigit(file_string[index + 1]) != 0) {
                        isNegative = true;
                    }
                    if (curr_equation != NULL && !hasOtherOperands && (isalpha(curr_equation[strlen(curr_equation) - 1]) != 0 || isdigit(curr_equation[strlen(curr_equation) - 1]) != 0)) {
                        appendToCurrEquation(&curr_equation, file_string[index]);
                    }
                    else {
                        int temp_Register = checkFree(temporary_registers, 10);
                        if (strchr(curr_equation, '+') != NULL) {
                            translateAddTemp(curr_equation, c_variables, temporary_registers, temp_Register);
                            free(curr_equation);
                            curr_equation = NULL;
                            // | represents the temporary register
                            appendToCurrEquation(&curr_equation, '|');
                            appendToCurrEquation(&curr_equation, file_string[index]);
                        }
                        // else if (strchr(curr_equation, '-') != NULL) {

                        // }
                        // else if (strchr(curr_equation, '*') != NULL) {

                        // }
                        // else if (strchr(curr_equation, '/') != NULL) {

                        // }
                        // else if (strchr(curr_equation, '%') != NULL) {

                        // }
                    }
                }
                // If the character is a digit
                else if (isdigit(file_string[index]) != 0) {
                    // Check if the last char is a digit or a operand. If either one, then append the digit to the equation
                    if (isdigit(curr_equation[strlen(curr_equation) - 1]) != 0 || curr_equation[strlen(curr_equation) - 1] == '+' || curr_equation[strlen(curr_equation) - 1] == '-' 
                        || curr_equation[strlen(curr_equation) - 1] == '*' || curr_equation[strlen(curr_equation) - 1] == '/' || curr_equation[strlen(curr_equation) - 1] == '%') {
                        if (!isNegative) {
                            appendToCurrEquation(&curr_equation, file_string[index]);
                        }
                        else {
                            appendToCurrEquation(&curr_equation, '-');
                            appendToCurrEquation(&curr_equation, file_string[index]);
                            isNegative = false;
                        }
                    }
                }
                printf("Current operation: %s\n", curr_equation);
                index++;
            }
            free(curr_equation);
        }
    }
}

void parseFile(const char *infileName) {
    FILE *infile = fopen(infileName, "r");

    if (infile == NULL) {
        printf("Error: Cannot open file %s\n", infileName);
        exit(1);
    }
    char file_string[128]; 
    
    CharIntPair c_variables[8];
    for (int i = 0; i < 8; i++) {
        c_variables[i].variable = ' ';
    }

    while (fgets(file_string, 128, infile) != NULL) {
        // File_string is each line and is passed into the translatetoMIPS function to be translated
        translatetoMIPS(file_string, c_variables);
    }
    // printCharIntPairs(c_variables);

    fclose(infile);
}

int main(int argc, char *argv[]) {
    // If the number of arguments is less than 2, print usage and exit
    if (argc < 2) {
        printf("Usage: %s <input file>\n", argv[0]);
        return 1;
    }

    // Initialize the input file name and call the parseFile function
    char *infileName = argv[1];
    parseFile(infileName);

    return 0;
}