#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);
int bSize;

char ***pathArr;
char toUpper(char c);
void initalize();
void printPathArr();
void checkWord(char** arr, char* word);
int checkPath(char** arr, char* word, int row, int col, int index);
int row = 0;
int col = 0;

// Main function, DO NOT MODIFY 	
int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <puzzle file name>\n", argv[0]);
        return 2;
    }
    int i, j;
    FILE *fptr;

    // Open file for reading puzzle
    fptr = fopen(argv[1], "r");
    if (fptr == NULL) {
        printf("Cannot Open Puzzle File!\n");
        return 0;
    }

    // Read the size of the puzzle block
    fscanf(fptr, "%d\n", &bSize);
    
    // Allocate space for the puzzle block and the word to be searched
    char **block = (char**)malloc(bSize * sizeof(char*));
    char *word = (char*)malloc(20 * sizeof(char));

    // Read puzzle block into 2D arrays
    for(i = 0; i < bSize; i++) {
        *(block + i) = (char*)malloc(bSize * sizeof(char));
        for (j = 0; j < bSize - 1; ++j) {
            fscanf(fptr, "%c ", *(block + i) + j);            
        }
        fscanf(fptr, "%c \n", *(block + i) + j);
    }
    fclose(fptr);

    printf("Enter the word to search: ");
    scanf("%s", word);
    
    // Print out original puzzle grid
    printf("\nPrinting puzzle before search:\n");
    printPuzzle(block);
    
    // Call searchPuzzle to the word in the puzzle
    searchPuzzle(block, word);
    
    return 0;
}

void printPuzzle(char** arr) {
    // This prints out the char for each columns. 
    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            printf("%c ", *(*(arr + i) + j));
        }
        printf("\n");
    }
    printf("\n");
}

void searchPuzzle(char** arr, char* word) {
    // This initalize the pathArr.
    initalize();
    checkWord(arr, word);
}

// This converts the character from a lowercase to uppercase with ASCII values.
char toUpper(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 32;
    }
    return c;
}

void initalize() {
    // This is going to allocate memory of the array.
    pathArr = (char***)malloc(bSize * sizeof(char**));

    for (int i = 0; i < bSize; i++) {
        // For each row, allocate columns
        *(pathArr + i) = (char**)malloc(bSize * sizeof(char*));
        for (int j = 0; j < bSize; j++) {
            // For each cell, allocate space for the string itself
            *(*(pathArr + i) + j) = (char*)malloc(20 * sizeof(char));
            // Initialize the string to "0"
            strcpy(*(*(pathArr + i) + j), "");
        }
    }

}

void printPathArr() {
    // Using this too print the pathArr.
    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            char* cell = *(*(pathArr + i) + j);
            // This prints "0" if the cell is empty.
            if (strlen(cell) == 0) {
                printf("0\t");
            } else {
                // This prints the path.
                printf("%s\t", cell);
            }
        }
         printf("\n");
    }
}

void checkWord(char** arr, char* word) {
    // This checks if the char is in the puzzle for each char in the string.
    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            if (toUpper(*(*(arr + i) + j)) == toUpper(*(word + 0))) {
                if (checkPath(arr, word, i, j, 0)) {
                    printf("Word found!\n");
                    printPathArr();
                    return; 
                }
            }
        }
    }
    printf("Word not found!\n");
}

int checkPath(char** arr, char* word , int row, int col, int index) {
    // if the word char doesn't make with the puzzle then its not a path.
    if (toUpper(*(*(arr + row) + col)) != toUpper(*(word + index))) {
        return 0;
    }

    // This gets the current path
    char* currentPath = *(*(pathArr + row) + col);

    // gets the length of the path before adding another path. This is for backtracking.
    int oldLen = strlen(currentPath);
    
    // This is the number of the next path. (its temporary)
    char newStep[10];

    sprintf(newStep, "%d", index + 1);

    // This adds the coma between the numbers if needed.
    if (oldLen == 0) {
        sprintf(currentPath, "%d", index + 1);
    } else {
        sprintf(currentPath + oldLen, ",%d", index + 1);
    }
    
    // if the word is complete than stop the recursive.
    if (index == strlen(word) - 1) {
        return 1;
    } 

    // Check top-left
    if (row > 0 && col > 0) {
        if (checkPath(arr, word, row - 1, col - 1, index + 1)) return 1;
    }
    // Check top-middle
    if (row > 0) {
        if (checkPath(arr, word, row - 1, col, index + 1)) return 1;
    }
    // Check top-right
    if (row > 0 && col < bSize - 1) {
        if (checkPath(arr, word, row - 1, col + 1, index + 1)) return 1;
    }
    // Check middle-left
    if (col > 0) {
        if (checkPath(arr, word, row, col - 1, index + 1)) return 1;
    }
    // Check middle-right
    if (col < bSize - 1) {
        if (checkPath(arr, word, row, col + 1, index + 1)) return 1;
    }
    // Check bottom-left
    if (col > 0 && row < bSize - 1) {
        if (checkPath(arr, word, row + 1, col - 1, index + 1)) return 1;
    }
    // Check bottom-middle
    if (row < bSize - 1) {
        if (checkPath(arr, word, row + 1, col, index + 1)) return 1;
    }
    // Check bottom-right
    if (col < bSize - 1 && row < bSize - 1) {
        if (checkPath(arr, word, row + 1, col + 1, index + 1)) return 1;
    }
    
    // Backtracke, go back to the pervious path.
    currentPath[oldLen] = '\0';

    return 0;
}