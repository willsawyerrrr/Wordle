#include <csse2310a1.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 52

int main(int argc, char* argv[]);
int validate_arguments(int argc, char* argv[]);
void play_game(int wordLength, int maxGuesses, char dictionaryName[]);
char* prompt_guess(int wordLength, int guesses);
int check_dictionary(char guess[], char dictionaryName[]);
char* report_matches(char answer[], char guess[]);

int main(int argc, char* argv[]) {
    int wordLength = 5;
    int maxGuesses = 6;
    char dictionaryName[] = "/usr/share/dict/words";

    if (!validate_arguments(argc, argv)) {
        printf("Usage: wordle [-len word-length] [-max max-guesses] "
                "[dictionary]\n");
        return 1;
    }

    // arguments are valid - use arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-len") == 0) {
            wordLength = atoi(argv[i + 1]);
            i++;
        } else if (strcmp(argv[i], "-max") == 0) {
            maxGuesses = atoi(argv[i + 1]);
            i++;
        } else {
            strcpy(dictionaryName, argv[i]);
        }
    }

    play_game(wordLength, maxGuesses, dictionaryName);
    return 0;
}

int validate_arguments(int argc, char* argv[]) {
    int wordSet = 0;
    int wordLength = 0;
    int guessesSet = 0;
    int maxGuesses = 0;
    int dictionarySet = 0;
    char dictionaryName[] = "";
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-len")) {
            if (i < argc - 1 && !wordSet) {
                wordLength = atoi(argv[++i]);
                wordSet = 1;
                if (wordLength < 3 || wordLength > 9) {
                    return 0;
                }
            } else {
                return 0;
            } 
        } else if (!strcmp(argv[i], "-max")) {
            if (i < argc - 1 && !guessesSet) {
                maxGuesses = atoi(argv[++i]);
                guessesSet = 1;
                if (maxGuesses < 3 || maxGuesses > 9) {
                    return 0;
                }
	        } else {
                return 0;
	        }
        } else if (((int) argv[i][0]) == '-') {
            return 0;
        } else if (atoi(argv[i]) == 0) {
            if (!dictionarySet) {
                strcpy(dictionaryName, argv[i]);
                FILE* dictionary = fopen(dictionaryName, "r");
                if (dictionary == NULL) {
                    printf("wordle:  dictionary file \"%s\" cannot be "
                            "opened\n", dictionaryName);
                    fclose(dictionary);
                    return 0;
                }
                fclose(dictionary);
                dictionarySet = 1;
            } else {
                return 0;
            }
        }
    }

    return 1;
}

void play_game(int wordLength, int maxGuesses, char dictionaryName[]) {
    printf("Welcome to Wordle!\n");
    char answer[wordLength];
    strcpy(answer, get_random_word(wordLength));
    char guess[wordLength];

    for (int i = maxGuesses; i > 0; i--) {
        if (i == 1) {
            printf("Enter a %d letter word (last attempt): ", wordLength);
        } else {
            printf("Enter a %d letter word (%d attempts remaining): ",
                    wordLength, i);
        }
         
        strcpy(guess, "");
        fgets(guess, MAX_WORD_LENGTH, stdin);
        guess[strcspn(guess, "\n")] = '\0';
        for (int i = 0; guess[i]; i++) {
            guess[i] = tolower(guess[i]);
        }

        if (feof(stdin)) { // no guess made
            break;
        } else if (strlen(guess) != wordLength) { // guess is wrong size
            i++;
            printf("Words must be %d letters long - try again.\n", wordLength);
        } else { // guess is correct size
            for (int j = 0; j < wordLength; j++) {
                if (!isalpha(guess[j])) { // non-alphabetical character present
                    i++;
                    printf("Words must contain only letters - try again.\n");
                    break;
                }
            }
             
            if (strcmp(guess, answer) == 0) { // guess is correct answer
                printf("Correct!\n");
                exit(0);
            } else { // guess not correct answer
                if (check_dictionary(guess, dictionaryName) == 1) {
                    printf("%s\n", report_matches(answer, guess));
                } else {
                    i++;
                    printf("Word not found in the dictionary - try again.\n");
                }
            }
        }
    }

    printf("Bad luck - the word is \"%s\".\n", answer);
    exit(3);
}

int check_dictionary(char guess[], char dictionaryName[]) {
    FILE* dictionary = fopen(dictionaryName, "r");
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

char* report_matches(char answer[], char guess[]) {
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

