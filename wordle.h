#include <stdio.h>

int main(int argc, char* argv[]);

/* validate_arguments()
 * --------------------
 * Determines whether the given command line arguments are valid.
 *
 * argc: number of command line arguments given, including program name
 * argv: array of command line arguments
 *
 * Returns: 1 if the command line arguments are valid; 0 otherwise
 */
int validate_arguments(int argc, char* argv[]);

/* get_dictionary()
 * ----------------
 * Creates a dictionary which contains only valid (purely alphabetical) words
 * of the correct length.
 *
 * readDictionaryPath: filepath of the full dictionary
 * writeDictionaryPath: filepath of the dictionary to create/update
 * wordLength: length of words to add to dictionary
 *
 * Returns: dictionary containing only valid words of the correct length
 * Errors: if the dictionary given by readDictionaryPath cannot be opened,
 *          the program exits with code 2
 */
FILE* get_dictionary(char readDictionaryPath[], char writeDictionaryPath[],
        int wordLength);

/* play_game()
 * ----------
 * Controls gameplay by managing calls to other functions.
 *
 * wordLength: length of the word to be guessed
 * maxGuesses: maximum number of guesses allowed
 * dictionary: dictionary file of valid words
 *
 * Errors: if the player uses all guesses without guessing the correct answer,
 *          the program exits with code 3
 */
void play_game(int wordLength, int maxGuesses, FILE* dictionary);

/* get_guess()
 * -----------
 * Prompts the player for a guess and stores the guess in the given guess
 * buffer.
 *
 * guess: buffer in which to store the guess
 * answer: the word to be guessed
 * wordLength: length of the word to be guessed
 * remainingGuesses: number of guesses remaining
 * dictionary: dictionary file
 *
 * Errors: if the player correctly guesses the answer, the program exits with
 *          code 0
 *         if EOF is reached (player presses Ctrl-D), the program exits with
 *          code 3
 */
void get_guess(char* guess, char* answer, int wordLength,
        int remainingGuesses, FILE* dictionary);

/* validate_guess()
 * ----------------
 * Determine whether the given guess is valid, based off its length and
 * characters.
 *
 * guess: word to check for validity
 * wordLength: length of the word to be guessed
 *
 * Returns: 1 if the given guess is valid; 0 otherwise
 */
int validate_guess(char* guess, int wordLength);

/* check_dictionary()
 * ------------------
 * Determine whether the given guess is in the given dictionary.
 *
 * guess: guess to check for in dictionary
 * dictionary: dictionary file
 *
 * Returns: 1 if the given guess in the given dictionary; 0 otherwise
 */
int check_dictionary(char* guess, FILE* dictionary);

/* report_matches()
 * ----------------
 * Prints matching letters in guess to the console:
 *     - correctly positioned letters are reported as uppercase
 *     - incorrectly positioned letters are reported as lowercase
 *     - unmatched letters are reported as with hyphens
 * 
 * guess: word guessed by the player
 * answer: word to be guessed
 */
void report_matches(char* guess, char* answer);

