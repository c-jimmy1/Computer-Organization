#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


void translatetoMIPS(file_string) {
    
}

void parseFile(const char *infileName) {
    FILE *infile = fopen(infileName, "r");

    if (infile == NULL) {
        printf("Error: Cannot open file %s\n", infileName);
        exit(1);
    }

    char file_string[128];
    
    while (fgets(file_string, 128, infile) != NULL) {
        // File_string is each line and is passed into the translatetoMIPS function to be translated
        translatetoMIPS(file_string);
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
    char *infileName = argv[1];
    parseFile(infileName);

    return 0;
}