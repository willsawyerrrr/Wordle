#include <csse2310a1.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void play_game(int wordLength, int maxGuesses, char* dictionaryName);

int main(int argc, char *argv[]) {
    int valid = 1;
    int wordLength = 5;
    int wordSet = 0;
    int maxGuesses = 6;
    int guessesSet = 0;
    char* dictionaryName = "";
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-len") == 0) {
            if (i < argc - 1 && !wordSet) {
                wordLength = atoi(argv[i + 1]);
                wordSet = 1;
            } else {
                valid = 0;
            } 
        } else if (strcmp(argv[i], "-max") == 0) {
            if (i < argc -1 && !guessesSet) {
                maxGuesses = atoi(argv[i + 1]);
                guessesSet = 1;
	        } else {
    	    	valid = 0;
	        }
        } else if (((int) argv[i][0]) == '-') {
            valid = 0;
        } else if (atoi(argv[i]) == 0) {
            dictionaryName = argv[i];
        }
    }

    if (!valid) {
        printf("Usage: wordle [-len word-length] [-max max-guesses] [dictionary]");
        return 1;
    }

    play_game(wordLength, maxGuesses, dictionaryName);
    return 0;
}

void play_game(int wordLength, int maxGuesses, char* dictionaryName) {
    printf("Welcome to Wordle!");
    char* answer  = get_random_word(wordLength);
}
