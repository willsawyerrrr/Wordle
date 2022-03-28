#include "wordle.h"
#include <csse2310a1.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* required buffer size to store maximum length word (50 char + '\n' + '\0') */
#define MAX_WORD_LENGTH 52
/* maximum allowed length of dictionary argument */
#define MAX_DICTIONARY_PATH_LENGTH 200
/* path to default dictionary */
#define DEFAULT_DICTIONARY_PATH "/usr/share/dict/words"
/* path to dictionary with only words of correct length */
#define CUSTOM_DICTIONARY_PATH "dictionary"

int main(int argc, char* argv[]) {
    // default game arguments
    int wordLength = 5;
    int maxGuesses = 6;
    char dictionaryPath[MAX_DICTIONARY_PATH_LENGTH] = DEFAULT_DICTIONARY_PATH;

    if (!validate_arguments(argc, argv)) {
        fprintf(stderr, "Usage: wordle [-len word-length] [-max max-guesses] "
                "[dictionary]\n");
        return 1;
    }

    // arguments are valid - use arguments
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-len")) {
            /* 
             * if the current argument is '-len', the next argument must be
             * the wordLength argument, since we know the arguments are valid
             *
             * uses argv[++i] to obtain the next argument (wordLength) and
             * ensure it is not found during the loop, since the update
             * statement of the for loop will increment i further before
             * another argument is checked to be '-len' or '-max'
             */
            wordLength = atoi(argv[++i]);
        } else if (!strcmp(argv[i], "-max")) {
            maxGuesses = atoi(argv[++i]);
        } else {
            strcpy(dictionaryPath, argv[i]);
        }
    }

    FILE* dictionary = get_dictionary(dictionaryPath, CUSTOM_DICTIONARY_PATH,
            wordLength);

    play_game(wordLength, maxGuesses, dictionary);
    fclose(dictionary);
    return 0;
}

int validate_arguments(int argc, char* argv[]) {
    int wordSet = 0;
    int guessesSet = 0;
    int dictionarySet = 0;
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-len")) { // current argument is '-len'
            if (!wordSet && ++i < argc) {
                /* 
                 * ensure wordLength is not set and that there is a next
                 * argument
                 *
                 * uses ++i to obtain the next index and then (below) argv[i]
                 * to obtain next argument (maxGuesses) and ensure it is not
                 * found during the loop, since the update statement of the for
                 * loop will increment i further before another argument is
                 * checked
                 */
                if (strlen(argv[i]) != 1 || atoi(argv[i]) < 3
                        || atoi(argv[i]) > 9) {
                    // wordLength argument is non-numeric or is out of bounds
                    return 0;
                }
                wordSet = 1;
            } else {
                return 0;
            } 
        } else if (!strcmp(argv[i], "-max")) {
            if (!guessesSet && ++i < argc) {
                if (strlen(argv[i]) != 1 || atoi(argv[i]) < 3
                        || atoi(argv[i]) > 9) {
                    return 0;
                }
                guessesSet = 1;
	    } else {
                return 0;
            }
        } else if (argv[i][0] == '-') {
            // starts with '-' but is not '-len' or '-max'
            return 0;
        } else if ((i + 1 == argc) && !atoi(argv[i])) {
            // ensure this is the final argument and that it is non-numeric
            if (!dictionarySet) {
                dictionarySet = 1;
            } else {
                return 0;
            }
        } else {
            return 0;
        }
    }

    // all checks have been passed - arguments are valid
    return 1;
}

FILE* get_dictionary(char readDictionaryPath[], char writeDictionaryPath[],
        int wordLength) {
    FILE* readDictionary = fopen(readDictionaryPath, "r");
    if (!readDictionary) {
        fprintf(stderr, "wordle: dictionary file \"%s\" cannot be "
                "opened\n", readDictionaryPath);
        exit(2);
    }

    char word[MAX_WORD_LENGTH];
    FILE* writeDictionary = fopen(writeDictionaryPath, "w+");

    int valid;
    while (fgets(word, MAX_WORD_LENGTH, readDictionary)) {
        // there are more words in the read dictionary
        word[strcspn(word, "\n")] = '\0'; // replace '\n' with '\0'
        if (strlen(word) == wordLength) {
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
    }

    fclose(readDictionary);
    return writeDictionary;
}

void play_game(int wordLength, int maxGuesses, FILE* dictionary) {
    printf("Welcome to Wordle!\n");
    char answer[wordLength];
    strcpy(answer, get_random_word(wordLength));
    char guess[wordLength];
    
    for (int current = 1; current <= maxGuesses; current++) {
        // +1 to below to account for program's zero-indexing
        int remainingGuesses = maxGuesses - current + 1;
        do {
            strcpy(guess, get_guess(answer, wordLength, remainingGuesses));
        } while (!validate_guess(guess, wordLength));

        if (check_dictionary(guess, dictionary)) {
            printf("%s\n", report_matches(guess, answer));
        } else {
            printf("Word not found in the dictionary - try again.\n");
            current--; // ensure invalid word does not waste a guess
        }
    }

    // word hasn't been guessed yet - player loses
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

    if (!fgets(guess, MAX_WORD_LENGTH, stdin)) { // no guess made
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

int validate_guess(char guess[], int wordLength) {
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

int check_dictionary(char guess[], FILE* dictionary) {
    rewind(dictionary); // ensure file is read from the beginning
    char word[MAX_WORD_LENGTH];

    while (fgets(word, MAX_WORD_LENGTH, dictionary)) {
        word[strcspn(word, "\n")] = 0;
        if (strcmp(word, guess) == 0) {
            return 1;
        }
    }

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

