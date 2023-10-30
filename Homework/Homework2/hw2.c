#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

/* Array of structs to hold the C variables, each struct has a char and an int. 
    The char is the variable name and the int is the value*/
    typedef struct {
        char c;
        int i;
    } CharIntPair;


// bool checkFree(CharIntPair (*c_variables)[8][2]) {

// }
void translatetoMIPS(char *file_string, CharIntPair c_variables[8]) {
    CharIntPair temporary_registers[10];
    int current_temp_register = 0;
    //printf("# %s", file_string);

    int i = 0;
    // Removing all spaces from the string
    while (file_string[i] != '\0') {
        if (file_string[i] == ' ') {
            for (int j = i; file_string[j] != '\0'; j++) {
                file_string[j] = file_string[j + 1];
            }
        }
        i++;
    }

    // Looping through the string and checking for keywords
    for (int j = 0; file_string[j] != '\0'; j++) {
        if (file_string[i] == ';' && file_string[i + 1] == '\0') {
            break;  // Exit the loop if a semicolon is at the end of the string
        }
        if (isalpha(file_string[i]) != 0 && file_string[i+1] == '=' && isalpha(file_string[i+2]) == 0) {
            printf("# %d", file_string[i]);
        }
    }

    c_variables[1].i = 5;
    
}

void parseFile(const char *infileName) {
    FILE *infile = fopen(infileName, "r");

    if (infile == NULL) {
        printf("Error: Cannot open file %s\n", infileName);
        exit(1);
    }
    char file_string[128]; 
    
    CharIntPair c_variables[8];
    while (fgets(file_string, 128, infile) != NULL) {
        // File_string is each line and is passed into the translatetoMIPS function to be translated
        translatetoMIPS(file_string, c_variables);
    }
    printf("%d\n ", c_variables[1].i);
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