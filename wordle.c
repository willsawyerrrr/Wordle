#include <csse2310a1.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_GUESS_LENGTH 52

int main(int argc, char *argv[]);
void play_game(int wordLength, int maxGuesses, char* dictionaryName);

int main(int argc, char *argv[]) {
    int valid = 1;
    int wordLength = 5;
    int wordSet = 0;
    int maxGuesses = 6;
    int guessesSet = 0;
    char* dictionaryName = "/usr/share/dict/words";

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-len") == 0) {
            if (i < argc - 1 && !wordSet) {
                wordLength = atoi(argv[++i]);
                wordSet = 1;
                if (wordLength < 3 || wordLength > 9) {
                    valid = 0;
                    break;
                }
            } else {
                valid = 0;
                break;
            } 
        } else if (strcmp(argv[i], "-max") == 0) {
            if (i < argc -1 && !guessesSet) {
                maxGuesses = atoi(argv[++i]);
                guessesSet = 1;
                if (maxGuesses < 3 || maxGuesses > 9) {
                    valid = 0;
                    break;
                }
	        } else {
    	    	valid = 0;
                break;
	        }
        } else if (((int) argv[i][0]) == '-') {
            valid = 0;
            break;
        } else if (atoi(argv[i]) == 0) {
            dictionaryName = argv[i];
        }
    }

    if (!valid) {
        printf("Usage: wordle [-len word-length] [-max max-guesses] [dictionary]\n");
        return 1;
    }

    play_game(wordLength, maxGuesses, dictionaryName);
    return 0;
}

void play_game(int wordLength, int maxGuesses, char* dictionaryName) {
    printf("Welcome to Wordle!\n");
    char* answer = get_random_word(wordLength);
    char* guess = malloc(wordLength);

    for (int i = 0; i < maxGuesses; i++) {
        printf("Enter a %d letter word (%d attempts remaining): ", wordLength, maxGuesses - i);
        guess = fgets(guess, MAX_GUESS_LENGTH, stdin);
        guess = strsep(&guess, "\n");

        if (feof(stdin)) { // no guess made
            break;
        } else if (strlen(guess) != wordLength) { // guess is wrong size
            i--;
            printf("Words must be %d letters long - try again.\n", wordLength);
        } else { // guess is correct size
            for (int j = 0; j < wordLength; j++) {
                if (!isalpha(guess[j])) { // non-alphabetical character present
                    i--;
                    printf("Words must contain only letters - try again.\n");
                    break;
                }
            }

            if (strcmp(guess, answer) == 0) { // guess is correct answer
                printf("Correct!\n");
                exit(0);
            } else { // guess not correct answer
                i--;
                printf("Word not found in the dictionary - try again.\n");
            }
        }
    }

    printf("Bad luck - the word is \"%s\".\n", answer);
    exit(3);
}

