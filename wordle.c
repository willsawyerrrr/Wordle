#include "wordle.h"
#include <csse2310a1.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 52

int main(int argc, char* argv[]) {
    int wordLength = 5;
    int maxGuesses = 6;
    char dictionaryPath[] = "/usr/share/dict/words";

    if (!validate_arguments(argc, argv)) {
        fprintf(stderr, "Usage: wordle [-len word-length] [-max max-guesses] "
                "[dictionary]\n");
        return 1;
    }

    // arguments are valid - use arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-len") == 0) {
            wordLength = atoi(argv[++i]);
        } else if (strcmp(argv[i], "-max") == 0) {
            maxGuesses = atoi(argv[++i]);
        } else {
            strcpy(dictionaryPath, argv[i]);
            FILE* dictionary = fopen(dictionaryPath, "r");
            if (!dictionary) {
                fprintf(stderr, "wordle: dictionary file \"%s\" cannot be "
                        "opened\n", dictionaryPath);
                return 2;
            }
            fclose(dictionary);
        }
    }

    play_game(wordLength, maxGuesses, dictionaryPath);
    return 0;
}

int validate_arguments(int argc, char* argv[]) {
    int wordSet = 0;
    int guessesSet = 0;
    int dictionarySet = 0;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-len")) {
            if (++i < argc && !wordSet) {
                if (atoi(argv[i]) < 3 || atoi(argv[i]) > 9) {
                    return 0;
                }
                wordSet = 1;
            } else {
                return 0;
            } 
        } else if (!strcmp(argv[i], "-max")) {
            if (++i < argc && !guessesSet) {
                if (atoi(argv[i]) < 3 || atoi(argv[i]) > 9) {
                    return 0;
                }
                guessesSet = 1;
	    } else {
                return 0;
            }
        } else if (argv[i][0] == '-') {
            return 0;
        } else if ((i + 1 == argc) && !atoi(argv[i])) {
            if (!dictionarySet) {
                dictionarySet = 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    }

    return 1;
}

void play_game(int wordLength, int maxGuesses, char dictionaryPath[]) {
    printf("Welcome to Wordle!\n");
    char answer[wordLength];
    char* guess = malloc(wordLength);
    strcpy(answer, get_random_word(wordLength));
    
    for (int current = 1; current <= maxGuesses; current++) {
        int remainingGuesses = maxGuesses - current + 1;
        do {
            guess = get_guess(answer, wordLength, remainingGuesses);
            if (!strcmp(guess, answer)) {
                printf("Correct!\n");
                exit(0);
            }
        } while (!validate_guess(guess, answer, wordLength, dictionaryPath));

        if (check_dictionary(guess, dictionaryPath)) {
            printf("%s\n", report_matches(guess, answer));
        } else {
            printf("Word not found in the dictionary - try again.\n");
            current--;
        } 
    }

    fprintf(stderr, "Bad luck - the word is \"%s\".\n", answer);
    exit(3);
}

char* get_guess(char answer[], int wordLength, int remainingGuesses) {
    char* guess = malloc(wordLength + 1);
    strcpy(guess, "");

    if (remainingGuesses == 1) {
        printf("Enter a %d letter word (last attempt): ", wordLength);
    } else {
        printf("Enter a %d letter word (%d attempts remaining): ",
                wordLength, remainingGuesses);
    }
    fgets(guess, MAX_WORD_LENGTH, stdin);
    guess[strcspn(guess, "\n")] = '\0';

    return guess;
}

int validate_guess(char guess[], char answer[], int wordLength,
        char dictionaryPath[]) {
    for (int i = 0; guess[i]; i++) {
        guess[i] = tolower(guess[i]);
    }

    if (feof(stdin)) { // no guess made
        return 0;
    } else if (strlen(guess) != wordLength) { // guess is wrong size
        printf("Words must be %d letters long - try again.\n", wordLength);
        return 0;
    }

    // word is correct length
    for (int j = 0; guess[j]; j++) {
        if (!isalpha(guess[j])) { // non-alpha character present
            printf("Words must contain only letters - try again.\n");
            return 0;
        }
    }
    return 1;
}

int check_dictionary(char guess[], char dictionaryPath[]) {
    FILE* dictionary = fopen(dictionaryPath, "r");
    char word[MAX_WORD_LENGTH];
    while (fgets(word, MAX_WORD_LENGTH, dictionary) != NULL) {
        word[strcspn(word, "\n")] = 0;
        for (int i = 0; word[i]; i++) {
            word[i] = tolower(word[i]);
        }
        if (strcmp(word, guess) == 0) {
            return 1;
        }
    }
    fclose(dictionary);
    return 0;
}

char* report_matches(char guess[], char answer[]) {
    char* matches = malloc(MAX_WORD_LENGTH);
    for (int i = 0; i < strlen(guess); i++) {
        matches[i] = '-';
    }
    matches[strlen(guess)] = '\0';
    for (int i = 0; guess[i]; i++) {
        if (answer[i] == guess[i]) {
            matches[i] = toupper(guess[i]);
        } else if (strchr(answer, guess[i]) != NULL) {
            matches[i] = guess[i];
        }
    }
    return matches;
}

