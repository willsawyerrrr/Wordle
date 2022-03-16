#include <csse2310a1.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 52

int main(int argc, char *argv[]);
void play_game(int wordLength, int maxGuesses, char* dictionaryName);
int check_dictionary(char* guess, char* dictionaryName);

int main(int argc, char *argv[]) {
    int valid = 1;
    int wordLength = 5;
    int wordSet = 0;
    int maxGuesses = 6;
    int guessesSet = 0;
    char* dictionaryName = "/usr/share/dict/words";
    int dictionarySet = 0;

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
            if (!dictionarySet) {
                dictionaryName = argv[i];
                if (fopen(dictionaryName, "r") == NULL) {
                    printf("wordle:  dictionary file \"%s\" cannot be opened\n", dictionaryName);
                    return 2;
                }
                dictionarySet = 1;
            } else {
                valid = 0;
                break;
            }
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
        guess = fgets(guess, MAX_WORD_LENGTH, stdin);
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
                if (check_dictionary(guess, dictionaryName) == 1) {
                    // report on matches
                } else {
                    i--;
                    printf("Word not found in the dictionary - try again.\n");
                }
            }
        }
    }

    printf("Bad luck - the word is \"%s\".\n", answer);
    exit(3);
}

int check_dictionary(char* guess, char* dictionaryName) {
    FILE *dictionary = fopen(dictionaryName, "r");
    char* word = malloc(strlen(guess));
    while (fgets(word, MAX_WORD_LENGTH, dictionary) != NULL) {
        word = strsep(&word, "\n");
        if (strcmp(word, guess) == 0) {
            return 1;
        }
    }
    return 0;
}
