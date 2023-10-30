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

void translatetoMIPS(char *file_string, CharIntPair c_variables[8]) {
    // Intialize an array of structs to hold the temporary registers
    CharIntPair temporary_registers[10];
    for (int i = 0; i < 8; i++) {
        temporary_registers[i].variable = ' ';
    }
    int current_temp_register = 0;
    
    printf("# %s", file_string);

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
        /* If the third character is a number, then it is a VARIABLE DECLARATION. 
        Assume that a const can only be the second operand */
        if (isalpha(file_string[2]) == 0) {
            int index = 2; // Start parsing from index 2
            int result = 0;
            while (file_string[index] != ';') {
                result = result * 10 + (file_string[index] - '0');
                index++;
            }
            declareVariable(c_variables, file_string[0], result);
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