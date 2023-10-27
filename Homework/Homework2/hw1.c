#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


void parseFile(const char *infileName) {
    FILE *infile = fopen(infileName, "r");

    if (infile == NULL) {
        printf("Error: Cannot open file %s\n", infileName);
        return 1;
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