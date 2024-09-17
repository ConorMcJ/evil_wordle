# Evil Wordle

## Overview

Evil Wordle is a command-line word guessing game inspired by the popular game Wordle. The objective is to guess a hidden word within a certain number of guesses. The game allows various modes, including viewing statistics, word lists, letter lists, and patterns. The user interacts with the game by guessing letters, and the game provides feedback on each guess.

## Features

- **Customizable Word Size**: Set the word length to any value between 2 and 29.
- **Customizable Number of Guesses**: Set the maximum number of guesses between 2 and 26.
- **Statistics Mode**: Display statistics about the dictionary and the current game.
- **Word List Mode**: Show the list of words of the specified length.
- **Letter List Mode**: Display previously guessed letters.
- **Pattern List Mode**: Show all patterns for the guessed letters.
- **Verbose Mode**: Enables all the above modes.

## Compilation

To compile the code, use a C compiler such as `gcc`. For example:

```sh
gcc -o evilwordle evilwordle.c
```

## Usage

Run the game from the command line with the following syntax:

```sh
./evilwordle [options]
```

### Options

- `-n <word_size>`: Specify the length of the word to guess (between 2 and 29). Default is 5.
- `-g <num_guesses>`: Specify the maximum number of guesses (between 2 and 26). Default is 26.
- `-s`: Enable statistics mode.
- `-w`: Enable word list mode.
- `-l`: Enable letter list mode.
- `-p`: Enable pattern list mode.
- `-v`: Enable verbose mode (activates all the above modes).

### Example

To run the game with a word size of 5, 6 guesses, and enable statistics and verbose modes:

```sh
./evilwordle -n 5 -g 6 -s -v
```

## File Requirements

- `dictionary.txt`: A text file containing a list of words, one per line. The game will filter words of the specified length from this file.

## Functions

- **`numDigits(int num)`**: Returns the number of digits in an integer.
- **`power(int base, int exp)`**: Computes the base raised to the power of exp.
- **`alphaSort(char str[])`**: Sorts a string alphabetically.
- **`prevChanges(char str[], char c)`**: Returns the number of characters in `str` that do not match `c`.
- **`addWord(char*** words, int* numWords, int* maxWords, char* newWord)`**: Adds a new word to a dynamic array of words, reallocating if necessary.
- **`addPattern(Pattern* patterns[], int* numPatterns, int* maxPatterns, Pattern newPattern)`**: Adds a new pattern to a dynamic array of patterns, reallocating if necessary.
- **`strNumMods(char* str1, char* str2)`**: Returns the number of character differences between two strings.
- **`findPatternCount(Pattern* p, char** words, int wordCount, int wordSize, char letter)`**: Finds and updates the count of a pattern in the word list.
- **`isPatternsPattern(Pattern p, Pattern* patterns, int patternCount)`**: Checks if a pattern already exists in the pattern list.
- **`strDiffInd(char* str1, char* str2)`**: Returns the index where two strings first differ.
- **`main(int argc, char* argv[])`**: The main function that runs the game.

## Memory Management

The program dynamically allocates memory for various structures. Ensure to deallocate memory properly to avoid memory leaks. The code includes free operations to handle dynamic memory.

## Troubleshooting

- **Invalid command-line arguments**: Make sure to provide valid arguments according to the usage instructions.

## Author

Conor McJannett  
CS 211  
Date: 10/07/2023
