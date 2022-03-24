#include "wordle.h"
#include <csse2310a1.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 52
#define MAX_DICTIONARY_PATH_LENGTH 200

int main(int argc, char* argv[]) {
    int wordLength = 5;
    int maxGuesses = 6;
    char dictionaryPath[MAX_DICTIONARY_PATH_LENGTH] = "/usr/share/dict/words";

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
        }
    }

    FILE* dictionary = fopen(dictionaryPath, "r");
    if (!dictionary) {
        fprintf(stderr, "wordle: dictionary file \"%s\" cannot be "
                "opened\n", dictionaryPath);
        return 2;
    }
    get_dictionary(dictionary, "dictionary", wordLength);

    play_game(wordLength, maxGuesses, dictionaryPath);
    fclose(dictionary);
    return 0;
}

int validate_arguments(int argc, char* argv[]) {
    int wordSet = 0;
    int guessesSet = 0;
    int dictionarySet = 0;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-len")) {
            if (++i < argc && !wordSet) {
                if (strlen(argv[i]) != 1 || atoi(argv[i]) < 3
                        || atoi(argv[i]) > 9) {
                    return 0;
                }
                wordSet = 1;
            } else {
                return 0;
            } 
        } else if (!strcmp(argv[i], "-max")) {
            if (++i < argc && !guessesSet) {
                if (strlen(argv[i]) != 1 || atoi(argv[i]) < 3
                        || atoi(argv[i]) > 9) {
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

void get_dictionary(FILE* readDictionary, char writeDictionaryPath[],
        int wordLength) {
    char word[MAX_WORD_LENGTH];
    FILE* writeDictionary = fopen(writeDictionaryPath, "w");

    int valid = 1;
    while (fgets(word, MAX_WORD_LENGTH, readDictionary)) {
        word[strcspn(word, "\n")] = '\0';
        valid = 1;
        for (int i = 0; word[i]; i++) {
            if (!isalpha(word[i])) {
                valid = 0;
                break;
            }
            word[i] = tolower(word[i]);
        }
        if (valid) {
            fprintf(writeDictionary, "%s\n", word);
        }
    }

    fclose(writeDictionary);
}

void play_game(int wordLength, int maxGuesses, char dictionaryPath[]) {
    printf("Welcome to Wordle!\n");
    char answer[wordLength];
    char* guess = malloc(wordLength);
    strcpy(answer, get_random_word(wordLength));
    for (int i = 0; answer[i]; i++) {
        answer[i] = tolower(answer[i]);
    }
    
    for (int current = 1; current <= maxGuesses; current++) {
        int remainingGuesses = maxGuesses - current + 1;
        do {
            guess = get_guess(answer, wordLength, remainingGuesses);
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
    char* guess = malloc(MAX_WORD_LENGTH);
    strcpy(guess, "");

    if (remainingGuesses == 1) {
        printf("Enter a %d letter word (last attempt):\n", wordLength);
    } else {
        printf("Enter a %d letter word (%d attempts remaining):\n",
                wordLength, remainingGuesses);
    }
    guess = fgets(guess, MAX_WORD_LENGTH, stdin);

    if (!guess) { // no guess made
        fprintf(stderr, "Bad luck - the word is \"%s\".\n", answer);
        exit(3);
    }

    guess[strcspn(guess, "\n")] = '\0';

    for (int i = 0; guess[i]; i++) {
        guess[i] = tolower(guess[i]);
    }

    if (!strcmp(guess, answer)) {
        printf("Correct!\n");
        exit(0);
    }

    return guess;
}

int validate_guess(char guess[], char answer[], int wordLength,
        char dictionaryPath[]) {
    if (strlen(guess) != wordLength) { // guess is wrong size
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

    while (fgets(word, MAX_WORD_LENGTH, dictionary)) {
        word[strcspn(word, "\n")] = 0;
        for (int i = 0; word[i]; i++) {
            word[i] = tolower(word[i]);
        }
        if (strcmp(word, guess) == 0) {
            fclose(dictionary);
            return 1;
        }
    }

    fclose(dictionary);
    return 0;
}

char* report_matches(char guess[], char answer[]) {
    char* matches = malloc(strlen(guess) + 1);
    char answerCopy[strlen(guess)];
    strcpy(answerCopy, answer);
    for (int i = 0; i < strlen(guess); i++) {
        matches[i] = '-';
    }
    matches[strlen(guess)] = '\0';

    // checks for correctly positioned letters
    for (int i = 0; guess[i]; i++) {
        if (answer[i] == guess[i]) {
            matches[i] = toupper(guess[i]);
            answerCopy[i] = '-';
            guess[i] = '-';
        }
    }

    // check incorrectly positioned letters
    for (int i = 0; guess[i]; i++) {
        for (int j = 0; guess[j]; j++) {
            char guessLetter = guess[i];
            char answerLetter = answerCopy[j];
            if (guessLetter != '-' && guessLetter == answerLetter) {
                matches[i] = guess[i];
                answerCopy[j] = '-';
                guess[i] = '-';
            }
        }
    }
    return matches;
}

