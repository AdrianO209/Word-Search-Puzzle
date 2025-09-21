#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Declarations of the two functions you will implement
// Feel free to declare any helper functions or global variables
void printPuzzle(char** arr);
void searchPuzzle(char** arr, char* word);
int bSize;

int **pathArr;
int ** visitedArr;
char toUpper(char c);
void initalize();
void printPathArr();
void checkWord(char** arr, char* word, int row, int col);
int checkPath(char** arr, char* word, int row, int col, int index);
int row = 0;
int col = 0;
int found = 0;

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
    checkWord(arr, word, 0, 0);
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
    pathArr = (int**)malloc(bSize * sizeof(int*));
    visitedArr = (int**)malloc(bSize * sizeof(int*));

    // This is going to allocate memory for each of the rows. 
    for (int i = 0; i < bSize; i++) {
        *(pathArr + i) = (int*)malloc(bSize * sizeof(int));
        *(visitedArr + i) = (int*)malloc(bSize * sizeof(int));
    }
    
    // This goes through each element and it initialize them to 0.
    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            *(*(pathArr + i) + j) = 0;
            *(*(visitedArr + i) + j) = 0;
        }
    }
}

void printPathArr() {
    // Using this too print the pathArr.
    for (int i = 0; i < bSize; i++) {
        for (int j = 0; j < bSize; j++) {
            printf("%d ", *(*(pathArr + i) + j));
        }
        printf("\n");
    }
}

void checkWord(char** arr, char* word , int row, int col) {
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
    if (index == strlen(word) - 1) {
        return 1;
    } 
    *(*(pathArr + row) + col) = index + 1;
    *(*(visitedArr + row) + col) = 1;

    // Check if we can play top-left
    if (row > 0 && col > 0) {
        if (toUpper(*(*(arr + (row - 1)) + (col - 1))) == toUpper(*(word + index + 1)) && *(*(visitedArr + (row - 1)) + (col - 1)) == 0) {
            if (checkPath(arr, word, row - 1, col - 1, index + 1)) {
                return 1;
            }
        }
    }

    // Check if we can play top-middle
    if (row > 0) {
        if (toUpper(*(*(arr + (row - 1)) + col)) == toUpper(*(word + index + 1)) && *(*(visitedArr + (row - 1)) + col) == 0) {
            if (checkPath(arr, word, row - 1, col, index + 1)) {
                return 1;
            }
        }
    }

    // Check if we can play top-right
    if (row > 0 && col < bSize - 1) {
        if (toUpper(*(*(arr + (row - 1)) + (col + 1))) == toUpper(*(word + index + 1)) && *(*(visitedArr + (row - 1)) + (col + 1)) == 0) {
            if (checkPath(arr, word, row - 1, col + 1, index + 1)) {
                return 1;
            }
        }

    }

    // Check if we can play middle-left
    if (col > 0) {
        if (toUpper(*(*(arr + row) + (col - 1))) == toUpper(*(word + index + 1)) && *(*(visitedArr + row) + (col - 1)) == 0) {
            if (checkPath(arr, word, row , col - 1, index + 1)) {
                return 1;
            }
        }
    }

    // Check if we can play middle-right
    if (col < bSize - 1) {
        if (toUpper(*(*(arr + row) + (col + 1))) == toUpper(*(word + index + 1)) && *(*(visitedArr + row) + (col + 1)) == 0) {
            if (checkPath(arr, word, row, col + 1, index + 1)) {
                return 1;
            }
        }
    }

    // Check if we can play bottom-left
    if (col > 0 && row < bSize - 1) {
        if (toUpper(*(*(arr + (row + 1)) + (col - 1))) == toUpper(*(word + index + 1)) && *(*(visitedArr + (row + 1)) + (col - 1)) == 0) {
            if (checkPath(arr, word, row + 1, col - 1, index + 1)){
                return 1;
            }
        }
    }

    // Check if we can play bottom-middle
    if (row < bSize - 1) {
        if (toUpper(*(*(arr + (row + 1)) + col)) == toUpper(*(word + index + 1)) && *(*(visitedArr + (row + 1)) + col) == 0) {
            if (checkPath(arr, word, row + 1, col, index + 1)) {
                return 1;
            }
        }
    }
    
    // Check if we can play bottom-right
    if (col < bSize - 1 && row < bSize - 1) {
        if (toUpper(*(*(arr + (row + 1)) + (col + 1))) == toUpper(*(word + index + 1)) && *(*(visitedArr + (row + 1)) + (col + 1)) == 0) {
            if (checkPath(arr, word, row + 1, col + 1, index + 1)) {
                return 1;
            }
        }
    }

    *(*(pathArr + row) + col) = 0;
    *(*(visitedArr + row) + col) = 0;
    return 0;
}