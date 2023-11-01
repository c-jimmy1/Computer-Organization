#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <ctype.h>

int checkFree(char *array[], int size) {
    for (int i = 0; i < size; i++) {
        if (array[i] == NULL) {
            return i;
        }
    }
    return -1;
}

int findIndex(char *c_variables[], int size, char target) {
    for (int i = 0; i < size; i++) {
        if (*c_variables[i] == target) {
            return i;
        }
    }
    return -1;
}

int isNumeric(const char *str) {
    char *endptr;  // Pointer to the character that caused conversion to stop
    errno = 0;     // Reset errno for error checking
    long num = strtol(str, &endptr, 10);  // Base 10 for decimal numbers
    
    // Check for conversion errors
    if ((errno == ERANGE && (num == LONG_MAX || num == LONG_MIN)) ||
        (errno != 0 && num == 0)) {
        return 0;  // Conversion failed
    }
    // Check if the entire string was consumed by strtol
    if (*endptr != '\0') {
        return 0;  // Not a valid integer (e.g., contains non-numeric characters)
    }
    return 1;  // Valid integer
}

void declareVariable(char *line[], char *c_variables[]) {
    int index = checkFree(c_variables, 8);
    c_variables[index] = line[0];
    printf("li $s%d, %s\n", index, line[2]);
}

int findPowers(int number, int *powers) {
    int exponent = 0, size = 0;
    while (number > 0) {
        if (number % 2 == 1) {
            powers[size++] = exponent;
        }
        number /= 2;
        exponent++;
    }
    if (number == 0) {
        return size;
    }
    return 0;
}

// Function to perform single operations, i.e. a = b + c or or a = b / c
void singleOperation(char *line[], char *c_variables[], char *temp_registers[]) {
    int indexLeft = 0;
    int indexRight = 0;
    int next_avaliable = checkFree(c_variables, 8);
    c_variables[next_avaliable] = line[0];
    if (*line[3] == '+') {
        if (isNumeric(line[4])) {
            int rightNum = atoi(line[4]);
            indexLeft = findIndex(c_variables, 8, *line[2]);
            printf("addi $s%d,$s%d,%d\n", next_avaliable, indexLeft, rightNum);
        }
        else {
            indexLeft = findIndex(c_variables, 8, *line[2]);
            indexRight = findIndex(c_variables, 8, *line[4]);
            printf("add $s%d,$s%d,$s%d\n", next_avaliable, indexLeft, indexRight);
        }
    }
    else if (*line[3] == '-') {
        if (isNumeric(line[4])) {
            int rightNum = atoi(line[4]);
            indexLeft = findIndex(c_variables, 8, *line[2]);
            printf("addi $s%d,$s%d,-%d\n", next_avaliable, indexLeft, rightNum);
        }
        else {
            indexLeft = findIndex(c_variables, 8, *line[2]);
            indexRight = findIndex(c_variables, 8, *line[4]);
            printf("sub $s%d,$s%d,$s%d\n", next_avaliable, indexLeft, indexRight);
        }
    }
    else if (*line[3] == '*') {
        if (isNumeric(line[4])) {
            indexLeft = findIndex(c_variables, 8, *line[2]);
            int rightNum = atoi(line[4]);
            int pow_size = 0;
            if (rightNum == 1) {
                int tempindex = checkFree(temp_registers, 10);
                temp_registers[tempindex] = "t";
                printf("move $t%d,$s%d\n", tempindex, indexLeft);
                printf("move $s%d,$t%d\n", next_avaliable, tempindex);
            }
            else if (rightNum < 0) {
                int tempindex = checkFree(temp_registers, 10);
                temp_registers[tempindex] = "t";
                printf("move $t%d,$s%d\n", tempindex, indexLeft);
                printf("sub $s%d,$zero,$t%d\n", next_avaliable, tempindex);
            }
            else if (rightNum == 0) {
                int tempindex = checkFree(temp_registers, 10);
                temp_registers[tempindex] = "t";
                printf("mult $t%d,$s%d\n", tempindex, indexLeft);
                int ansindex = checkFree(temp_registers, 10);
                temp_registers[ansindex] = "t";
                printf("mflo $t%d\n", ansindex);
                printf("li $s%d,0\n", next_avaliable);
            }
            else {
                int *powers = malloc(50 * sizeof(int));
                //int pow_size = findPowers(*line[4], powers);
                if (pow_size > 1)
                for (int i = pow_size-1; i >= 0; i--) {
                    printf("sll $s%d,$s%d,%d\n", indexLeft, indexLeft, powers[i]);
                }
            } 
        }
        else {
            indexLeft = findIndex(c_variables, 8, *line[2]);
            indexRight = findIndex(c_variables, 8, *line[4]);
            printf("mult $s%d,$s%d\n", indexLeft, indexRight);
            printf("mflo $s%d\n", next_avaliable);
        }
    }
}

void translatetoMIPS(char *line[], char *c_variables[], int line_size) {
    // Initialize temp_registers array to NULL
    char *temp_registers[10];
    for (int i = 0; i < 10; i++) {
        temp_registers[i] = NULL;
    }
    if (isalpha(*line[0]) != 0 && *line[1] == '=') {
        if (isNumeric(line[2])) {
            declareVariable(line, c_variables);
        }
        else {
            if (line_size == 5) {
                singleOperation(line, c_variables, temp_registers);
            }

        }
    }
}

// Function to print the original C code line as a comment
void printOriginal(char *line_string) {
    char *newline = strchr(line_string, '\n');
        if (newline != NULL) {
            *newline = '\0';
        }
        printf("# %s\n", line_string); 
} 

void parseFile(const char *infileName) {
    FILE *infile = fopen(infileName, "r");

    if (infile == NULL) {
        printf("Error: Cannot open file %s\n", infileName);
        exit(1);
    }

    char line_string[128];
    
    // Initialize c_variables array to NULL
    char *c_variables[8];
    for (int i = 0; i < 8; i++) {
        c_variables[i] = NULL;
    }

    // Read each line of the file using fgets, and tokenize the line by space, and convert the tokens to an Array
    while (fgets(line_string, 128, infile) != NULL) {        
        printOriginal(line_string);
        line_string[strcspn(line_string, ";")] = 0; // Remove the semicolon from the line
        char *token = strtok(line_string, " "); // Tokenize the line by space
        int tokenCount = 0;
        char *line[128];
        
        // Initialize line array to NULL
        for (int i = 0; i < 128; i++) {
            line[i] = NULL;
        }

        while (token != NULL) {
            line[tokenCount] = strdup(token); // Store the token in the array
            tokenCount++;
            token = strtok(NULL, " "); // Move to the next token
        }
        
        translatetoMIPS(line, c_variables, tokenCount);
          
    }

    fclose(infile);
}

int main(int argc, char *argv[]) {
    // If the number of arguments is less than 2, print usage and exit
    if (argc < 2) {
        printf("Usage: %s <input file>\n", argv[0]);
        return 1;
    }

    // Initialize the input file name and call the parseFile function
    const char *infileName = argv[1];
    parseFile(infileName);

    return 0;
}
