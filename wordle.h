#include <stdio.h>

/**
 * Entry point of this program, used to run a game of Wordle.
 *
 * Usage: ./wordle [-len word-length] [-max max-guesses] [dictionary]
 */
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
FILE* get_dictionary(FILE* readDictionary, char writeDictionaryPath[],
        int wordLength);

/**
 * Control gameplay: generate answer; prompt for guesses; determine validity;
 * return matches; end game.
 *
 * @param wordLength length of the word to be guessed
 * @param maxGuesses maximum number of guesses allowed
 * @param dictionaryPath path to dictionary file
 */
void play_game(int wordLength, int maxGuesses, char dictionaryPath[]);

/**
 * Prompt user for a guess.
 *
 * @param wordLength length of the word to be guessed
 * @param maxGuesses maximum number of guesses allowed
 * @param remainingGuesses number of guesses remaining
 *
 * @return pointer to guess
 */
char* get_guess(char answer[], int wordLength, int remainingGuesses);

/**
 * Determine whether given guess is valid.
 *
 * @param guess word to check for validity
 * @param answer word to be guessed
 * @param wordLength length of the word to be guessed
 * @param dictionaryPath path to dictionary file
 *
 * @return 1 if valid; 0 otherwise
 */
int validate_guess(char guess[], char answer[], int wordLength,
        char dictionaryPath[]);

/**
 * Determine whether given guess is in the dictionary at given name.
 *
 * @param guess word to check in dictionary
 * @param dictionaryPath path to dictionary file
 *
 * @return 1 if guess in dictionary; 0 otherwise
 */
int check_dictionary(char guess[], char dictionaryPath[]);

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

