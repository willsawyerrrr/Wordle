#include <stdio.h>

int main(int argc, char* argv[]);

/**
 * Determine whether given command line arguments are valid.
 *
 * @param argc number of command line arguments given, including program name
 * @param argv pointer to array of command line arguments
 *
 * @return 1 if valid; 0 otherwise
 */
int validate_arguments(int argc, char* argv[]);

/**
 *
 */
FILE* get_dictionary(char readDictionaryPath[], char writeDictionaryPath[],
        int wordLength);

/**
 * Control gameplay: generate answer; prompt for guesses; determine validity;
 * return matches; end game.
 *
 * @param wordLength length of the word to be guessed
 * @param maxGuesses maximum number of guesses allowed
 * @param dictionary dictionary file
 */
void play_game(int wordLength, int maxGuesses, FILE* dictionary);

/**
 * Prompt player for a guess. Guess is stored to the address given by the guess
 * parameter.
 *
 * @param guess pointer to string in which to place the guess
 * @param answer the word to be guessed
 * @param wordLength length of the word to be guessed
 * @param remainingGuesses number of guesses remaining
 */
void get_guess(char* guess, char* answer, int wordLength,
        int remainingGuesses);

/**
 * Determine whether given guess is valid, based off its length and characters.
 *
 * @param guess word to check for validity
 * @param wordLength length of the word to be guessed
 *
 * @return 1 if valid; 0 otherwise
 */
int validate_guess(char* guess, int wordLength);

/**
 * Determine whether given guess is in the dictionary at given name.
 *
 * @param guess word to check in dictionary
 * @param dictionary dictionary file
 *
 * @return 1 if guess in dictionary; 0 otherwise
 */
int check_dictionary(char* guess, FILE* dictionary);

/**
 * Report matching letters in guess:
 *     - correctly positioned letters are uppercase 'A'
 *     - incorrectly positioned letters are lowercase 'a'
 *     - unmatched letters are replaced with hyphens '-'
 * 
 * @param guess word to be checked for validity
 * @param answer word to be guessed
 * 
 * @return pointer to string of matches
 */
char* report_matches(char guess[], char answer[]);

