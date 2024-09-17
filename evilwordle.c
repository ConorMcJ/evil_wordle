// evilwordle.c
//
// Conor McJannett
// CS 211
// 10/07/2023
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

typedef struct Pattern_struct {
    char* pat;  //string pattern - exs: "hello", "-ell-", "-----"
    int count;  //frequency of pattern in the current word list
    int changes;//number of occurences of new letter in this pattern
} Pattern;

// Returns number of digits in an integer
int numDigits(int num) {
    if (num/10 == 0) return 1;
    return 1 + numDigits(num/10);
}

// Recursively returns integer raised to some integer power
int power(int base, int exp) {
    if (exp < 0) return 0;
    if (exp < 1) return 1;
    return base * power(base, exp-1);
}

// Sorts cstring contents alphabetically
void alphaSort(char str[]) {
    int a, b;
    for (int i = 1; i < strlen(str); i++) {
        a = str[i];
        b = i-1;
        while (b >= 0 && str[b] > a) {
            str[b+1] = str[b];
            b--;
        }
        str[b+1] = a;
    }
}

// Returns number of characters in str that match character c
int prevChanges(char str[], char c) {
    int num = 0;
    for (int i = 0; i < strlen(str); i++) {
        if (str[i] != c) num++;
    }
    return num;
}

//-------------------------------------------------------------------
// TODO - Task II: write addWord() function, which adds a new word to
//      a dynamic heap-allocated array of C-strings; heap-space must
//      be allocated for the new word inside this function, then copy
//      the chars from newWord to the newly allocated heap-space; if
//      adding the new word pushes numWords above maxWords, then
//      realloate the words array with double the capacity
//      parameters:
//          char*** words - array of C-strings passed-by-pointer
//              note: *words is an array of pointers
//                    **words is an array of chars
//                    ***words is a char
//                    (*words)[0] is the 1st C-string in the array
//                    (*words)[1] is the 2nd C-string in the array
//                    (*words)[0][0] is 1st char of 1st C-string
//                    (*words)[1][2] is 3rd char of 2nd C-string
//                    etc.
//          int* numWords - current # of words in *words array
//               note: numWords will increase by 1 here,
//                     so it is passed-by-pointer
//          int* maxWords - current capacity of *words array
//               note: maxWords may increase by x2 here,
//                     so it is passed-by-pointer
//          char* newWord - C-string word to be added to words
//               note: newWord is a C-string, automatically a pointer
//-------------------------------------------------------------------
void addWord(char*** words, int* numWords, int* maxWords, char* newWord) {
    (*numWords)++;
    
    // If new word exceeds max words, reallocate words
    if (*numWords > *maxWords) {
        char** tempWords = *words;
        (*maxWords) *= 2;
        *words = (char**)malloc((*maxWords)*sizeof(char*));
        for (int i = 0; i < (*maxWords)/2; i++) {
            (*words)[i] = (char*)malloc((strlen(tempWords[i])+1)*sizeof(char));
            strcpy((*words)[i], tempWords[i]);
            free(tempWords[i]);
        }
        free(tempWords);
        tempWords = NULL;
    }
    
    // Copy new word to words array
    (*words)[(*numWords)-1] = (char*)malloc((strlen(newWord)+1)*sizeof(char));
    strcpy((*words)[(*numWords)-1], newWord);
}

void addPattern(Pattern* patterns[], int* numPatterns,
                int* maxPatterns, Pattern newPattern) {
    (*numPatterns)++;
//    printf("\nnewPattern: %s\n", newPattern.pat);
    // If new pattern exceeds max patterns, reallocate patternList
    if (*numPatterns > *maxPatterns) {
        Pattern* tempPatterns = *patterns;
        (*maxPatterns) *= 2;
        *patterns = (Pattern*)malloc((*maxPatterns)*sizeof(Pattern));
        for (int i = 0; i < (*maxPatterns)/2; i++) {
            (*patterns)[i].count = tempPatterns[i].count;
            (*patterns)[i].changes = tempPatterns[i].changes;
            (*patterns)[i].pat = (char*)malloc((strlen(tempPatterns[i].pat)
                                                +1)*sizeof(char));
            strcpy((*patterns)[i].pat, tempPatterns[i].pat);
            free(tempPatterns[i].pat);
        }
        free(tempPatterns);
        tempPatterns = NULL;
    }
    
    (*patterns)[(*numPatterns)-1].count = newPattern.count;
    (*patterns)[(*numPatterns)-1].changes = newPattern.changes;
    (*patterns)[(*numPatterns)-1].pat = (char*)malloc((strlen
                                                       (newPattern.pat)
                                                       +1)*sizeof(char));
//    printf("newPattern: %s\n", newPattern.pat);
    strcpy((*patterns)[(*numPatterns)-1].pat, newPattern.pat);
//    printf("patterns[%d] = %s\n\n", (*numPatterns)-1, (*patterns)[(*numPatterns)-1].pat);
}

//-------------------------------------------------------------------
// TODO - Task V: write the strNumMods() function, which
//      returns count of character differences between two strings;
//      includes extra characters in longer string if different lengths
// Exs: str1 = magic, str2 = magic, returns 0
//      str1 = wands, str2 = wants, returns 1
//      str1 = magic, str2 = wands, returns 4
//      str1 = magic, str2 = mag, returns 2
//      str1 = magic, str2 = magicwand, returns 4
//      str1 = magic, str2 = darkmagic, returns 8
//-------------------------------------------------------------------
int strNumMods(char* str1, char* str2) {
    
    // Verify str1 and str2 have contents != NULL
    if (str1 == NULL || str2 == NULL) return -1;
    
    // Initialize local variables
    int charCmp = 0;
    char* shortWord;
    char* longWord;
    
    // Assign short and long word pointers
    if (strlen(str1) > strlen(str2)) {
        shortWord = str2;
        longWord = str1;
    } else {
        shortWord = str1;
        longWord = str2;
    }
    charCmp = (int)(strlen(longWord)-strlen(shortWord));
    // Iterate through each character in shortWord, compare to longWord
    for (int i = 0; i < strlen(shortWord); i++) {
        if (shortWord[i] != longWord[i]) charCmp++;
    }
    return charCmp;
}

// Find the number of words pattern p matches, assign to p.count
void findPatternCount(Pattern* p, char** words, int wordCount,
                     int wordSize, char letter) {
//    printf("patternCount before: %d\n", (*p).count);
    for (int i = 0; i < wordCount; i++) {
        if (strNumMods((*p).pat, words[i]) ==
            wordSize-(prevChanges((*p).pat, '-'))) {
            ((*p).count)++;
            for (int k = 0; k < wordSize; k++) {
                if (words[i][k] == letter &&
                    ((*p).pat)[k] == '-') {
                    ((*p).count)--;
                    break;
                }
            }
        }
        }
//    printf("patternCount after: %d\n", (*p).count);
}

// Return true if pattern exists in patterns (except at index 0)
bool isPatternsPattern(Pattern p, Pattern* patterns, int patternCount) {
    for (int i = 1; i < patternCount; i++) {
        if (strNumMods(patterns[i].pat, p.pat) == 0) return true;
    }
    return false;
}

//-------------------------------------------------------------------
// TODO - Task VI: write the strDiffInd() function, which
//      returns index of the character where two strings first differ, &
//      returns strlen(str1) = strlen(str2) if no differences
// Exs: str1 = magic, str2 = magic, returns 5
//      str1 = wands, str2 = wants, returns 3
//      str1 = magic, str2 = wands, returns 0
//      str1 = magic, str2 = mag, returns 3
//      str1 = magic, str2 = magicwand, returns 5
//      str1 = magic, str2 = darkmagic, returns 0
//-------------------------------------------------------------------
int strDiffInd(char* str1, char* str2) {
    for (int i = 0; i < strlen(str1); i++) {
        if (str1[i] != str2[i]) return i;
    }
    return (int)strlen(str1);
}

int main(int argc, char* argv[]) {
    
    // Initialize settings variables with default values
    int wordSize = 5;
    int numGuesses = 26;
    bool statsMode = false;
    bool wordListMode = false;
    bool letterListMode = false;
    bool patternListMode = false;
    bool verboseMode = false;
//    bool extensionMode = false;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-n") == 0 && argv[i+1] == NULL) {
            printf("Invalid word size.\nTerminating program...");
            return -1;
        } else if (strcmp(argv[i], "-n") == 0) {
            for (int k = 0; k < strlen(argv[i+1]); k++) {
                if (!isdigit(argv[i+1][k])) {
                    printf("Invalid word size.\nTerminating program...");
                    return -1;
                }
            }
            wordSize = atoi(argv[i+1]);
            if (wordSize < 2 || wordSize > 29) {
                printf("Invalid word size.\nTerminating program...");
                return -1;
            }
            i++;
        } else if (strcmp(argv[i], "-g") == 0 && argv[i+1] == NULL) {
            printf("Invalid number of guesses.\nTerminating program...");
            return -1;
        } else if (strcmp(argv[i], "-g") == 0) {
            for (int k = 0; k < strlen(argv[i+1]); k++) {
                if (!isdigit(argv[i+1][k])) {
                    printf("Invalid number of guesses.\n");
                    printf("Terminating program...");
                    return -1;
                }
            }
            numGuesses = atoi(argv[i+1]);
            if (numGuesses < 2 || numGuesses > 26) {
                printf("Invalid number of guesses.\nTerminating program...");
                return -1;
            }
            i++;
        } else if (strcmp(argv[i], "-s") == 0) {
            statsMode = true;
        } else if (strcmp(argv[i], "-w") == 0) {
            wordListMode = true;
        } else if (strcmp(argv[i], "-l") == 0) {
            letterListMode = true;
        } else if (strcmp(argv[i], "-p") == 0) {
            patternListMode = true;
        } else if (strcmp(argv[i], "-v") == 0) {
            verboseMode = true;
        }
//        else if (strcmp(argv[i], "-x") == 0) {
//            extensionMode = true;
//        }
        else {
            printf("Invalid command-line argument.\nTerminating program...");
            return -1;
        }
    }
    
    /* If verbose, turn on statsMode, wordListMode,
       letterListMode, and patternListMode */
    if (verboseMode) {
        statsMode = true;
        wordListMode = true;
        letterListMode = true;
        patternListMode = true;
    }
    
    // Task I: handle command-line arguments
    printf("Welcome to the (Evil) Word Guessing Game!\n");
    printf("\nGame Settings:\n");
    printf("  Word Size = %d\n", wordSize);
    printf("  Number of Guesses = %d\n", numGuesses);
    printf("  View Stats Mode = ");
    statsMode ? printf("ON\n") : printf("OFF\n");
    printf("  View Word List Mode = ");
    wordListMode ? printf("ON\n") : printf("OFF\n");
    printf("  View Letter List Mode = ");
    letterListMode ? printf("ON\n") : printf("OFF\n");
    printf("  View Pattern List Mode = ");
    patternListMode ? printf("ON\n") : printf("OFF\n");
    
    // Task III: file-read the word list from dictionary.txt
    int capacity = 4;
    int wordCount = 0;
    char** wordList = (char**)malloc(capacity*sizeof(char*));
    FILE* inFile = fopen("dictionary.txt", "r");
    if (inFile == NULL) {
        printf("dictionary.txt not found.\nTerminating program...");
        free(wordList);
        return -1;
    }
    char word[30];
    while (!feof(inFile)) {
        fscanf(inFile, "%s", word);
        if (strlen(word) == wordSize) {
            addWord(&wordList, &wordCount, &capacity, word);
        }
    }
    fclose(inFile);
    inFile = NULL;

    //-------------------------------------------------------------------
    // TODO - Task IV: display dictionary stats [-s] & word list [-w]
    //                 if the proper command-line flags are turned ON;
    //                 see sample output for proper formatting
    //-------------------------------------------------------------------
    
    if (statsMode) {
        printf("The dictionary contains 127142 words total.\n");
        printf("The longest word floccinaucinihilipilification "
               "has 29 chars.\n");
        printf("The dictionary contains %d words ", wordCount);
        printf("of length %d.\n", wordSize);
        printf("Max size of the dynamic words array is %d.\n", capacity);
    }
    if (wordListMode) {
        printf("Words of length %d:\n", wordSize);
        if (wordCount == 0) {
            printf("Dictionary has no words of length %d.\n", wordSize);
            printf("Terminating program...");
            free(wordList);
            return -1;
        }
        for (int i = 0; i < wordCount; i++) {
            printf("  %s\n", wordList[i]);
        }
    }
    printf("The word pattern is: ");
    for (int i = 0; i < wordSize; i++) {
        printf("-");
    }
    printf("\n");

    //-------------------------------------------------------------------
    //Initialize variables for gameplay loop
    bool isValid;
    char letter;
    char prevGuessedList[27];
    prevGuessedList[0] = '\0';
    int tempCapacity = 4;
    int tempWordCount = 0;
    int patternCapacity = 4;
    int patternCount = 0;
    char** tempWordList;
    Pattern* patternList = (Pattern*)malloc(patternCapacity*sizeof(Pattern));
    Pattern tempPattern;
    
    // Initialize pattern at index 0
    tempPattern.count = wordCount;
    tempPattern.changes = 0;
    tempPattern.pat = (char*)malloc((wordSize+1)*sizeof(char));
    for (int i = 0; i < wordSize; i++) {
        tempPattern.pat[i] = '-';
    }
    tempPattern.pat[wordSize] = '\0';
    addPattern(&patternList, &patternCount, &patternCapacity, tempPattern);
    
    // Gameplay loop
    while (true) {
        printf("\nNumber of guesses remaining: %d\n", numGuesses);
        if (letterListMode) {
            printf("Previously guessed letters: ");
            for (int i = 0; i < strlen(prevGuessedList); i++) {
                printf("%c ", prevGuessedList[i]);
            }
            printf("\n");
        }
        
        // Take user input
        printf("Guess a letter (# to end game): ");
        scanf(" %c", &letter);
        while (!isalpha(letter) || isupper(letter) ||
               strchr(prevGuessedList, letter) != NULL) {
            if (letter == '#') {
                printf("Terminating game...\n");
                break;
            } else if (!isalpha(letter) || isupper(letter)) {
                printf("Invalid letter...\n");
            } else {
                printf("Letter previously guessed...\n");
            }
            printf("Guess a letter (# to end game): ");
            scanf(" %c", &letter);
        }
        if (letter == '#') break;
        
        // Add letter to prev guessed list
        strncat(prevGuessedList, &letter, 1);
        alphaSort(prevGuessedList);
        
        // Initialize every pattern with corresponding count and change values
        for (int i = 0; i < wordCount; i++) {
            tempPattern.count = 0;
            tempPattern.changes = patternList[0].changes;
            strcpy(tempPattern.pat, patternList[0].pat);
//            printf("tempPattern: %s\n", tempPattern.pat);
            for (int j = 0; j < wordSize; j++) {
                if (wordList[i][j] == letter && tempPattern.pat[j] == '-') {
                    tempPattern.pat[j] = letter;
                    (tempPattern.changes)++;
//                    printf("tempPattern: %s\n", tempPattern.pat);
                }
            }
//            printf("\nFinal tempPattern: %s\n", tempPattern.pat);
            
            // If pattern is not previously initialized, add to patternList
            if (!isPatternsPattern(tempPattern, patternList, patternCount)) {
                findPatternCount(&tempPattern, wordList,
                                 wordCount, wordSize, letter);
//                printf("\ntempPattern before copying: %s\n", tempPattern.pat);
                addPattern(&patternList, &patternCount,
                           &patternCapacity, tempPattern);
//                printf("patternList[%d].pat: %s\n", patternCount-1, patternList[patternCount-1].pat);
            }
        }
        
        // Subtracts words containing new letter from original pattern
        for (int i = 0; i < wordCount; i++) {
            if (strchr(wordList[i], letter) != NULL) {
                patternList[0].count--;
            }
        }
        
        // If -p in argv, print patterns for new letter
        if (patternListMode) {
            printf("All patterns for letter %c: \n", letter);
            for (int i = 1; i < patternCount; i++) {
                printf("  %s \t", patternList[i].pat);
                printf("count = %d \t", patternList[i].count);
                printf("changes = %d\n", patternList[i].changes);
            }
        }
        
        // Determine a pattern with the highest count and lowest change values
        // Write optimal pattern to patternList[0]
//        printf("original pattern: %s\n", patternList[0].pat);
        for (int i = 1; i < patternCount; i++) {
            if (patternList[i].count > patternList[0].count) {
                patternList[0].count = patternList[i].count;
                patternList[0].changes = patternList[i].changes;
                strcpy(patternList[0].pat, patternList[i].pat);
            } else if (patternList[i].count == patternList[0].count &&
                       patternList[i].changes < patternList[0].changes) {
                patternList[0].count = patternList[i].count;
                patternList[0].changes = patternList[i].changes;
                strcpy(patternList[0].pat, patternList[i].pat);
            } else if (patternList[i].count == patternList[0].count &&
                       patternList[i].changes <= patternList[0].changes &&
                       strDiffInd(patternList[i].pat, patternList[0].pat) <
                       strchr(patternList[0].pat, letter)-(patternList[0].pat)) {
                patternList[0].count = patternList[i].count;
                patternList[0].changes = patternList[i].changes;
                strcpy(patternList[0].pat, patternList[i].pat);
            }
        }
        
        // Analyze guess, check win/loss conditions
        if (strchr(patternList[0].pat, letter) != NULL) {
            printf("Good guess! The word has at least one %c.\n", letter);
            if (statsMode) { // If -s in argv, print possible words remaining
                printf("Number of possible words remaining: %d\n",
                       patternList[0].count);
            }
            if (strchr(patternList[0].pat, '-') == NULL) {
                printf("The word pattern is: %s\n", patternList[0].pat);
                printf("\nYou solved the word!\n");
                printf("The word is: %s\nGame over.\n", patternList[0].pat);
                break;
            }
        } else {
            printf("Oops, there are no %c's. You used a guess.\n", letter);
            if (statsMode) {
                printf("Number of possible words remaining: %d\n",
                       patternList[0].count);
            }
            numGuesses--;
        }
        if (numGuesses == 0) {
            if (wordListMode) {
                printf("Possible words are now:\n");
                for (int i = 0; i < wordCount; i++) {
                    printf("  %s\n", wordList[i]);
                }
            }
            printf("The word pattern is: %s\n", patternList[0].pat);
            printf("\nYou ran out of guesses and did not solve the word.\n");
            for (int i = 0; i < wordCount; i++) {
                if (strNumMods(patternList[0].pat, wordList[i]) ==
                    wordSize-(prevChanges(patternList[0].pat, '-'))) {
                    printf("The word is: %s\nGame over.", wordList[i]);
                }
            }
            break;
        }
        // Update wordList
        tempWordList = (char**)malloc(tempCapacity*sizeof(char*));
        if (tempWordList == NULL) {
            printf("tempWordList failed to initialize\n");
            printf("Terminating program...");
            break;
        }
        for (int i = 0; i < wordCount; i++) {
            if (strNumMods(wordList[i], patternList[0].pat) ==
                wordSize-(prevChanges(patternList[0].pat, '-'))) {
                isValid = true;
                for (int j = 0; j < wordSize; j++) {
                    if ((patternList[0].pat)[j] == '-' &&
                        wordList[i][j] == letter) {
                        isValid = false;
                        break;
                    }
                }
                if (isValid) {
                    addWord(&tempWordList, &tempWordCount,
                            &tempCapacity, wordList[i]);
                }
            }
        }
        for (int i = 0; i < wordCount; i++) {
            free(wordList[i]);
        }
        free(wordList);
        capacity = tempCapacity;
        wordCount = tempWordCount;
        wordList = tempWordList;
        tempCapacity = 4;
        tempWordCount = 0;
        if (wordListMode) {
            printf("Possible words are now:\n");
            for (int i = 0; i < wordCount; i++) {
                printf("  %s\n", wordList[i]);
            }
        }
        
        // Reset patternList
        tempPattern.count = patternList[0].count;
        tempPattern.changes = 0;
        strcpy(tempPattern.pat, patternList[0].pat);
        free(patternList[0].pat);
        for (int i = 1; i < patternCount; i++) {
            free(patternList[i].pat);
        }
        free(patternList);
        patternCapacity = 4;
        patternCount = 0;
        patternList = (Pattern*)malloc(patternCapacity*sizeof(Pattern));
        addPattern(&patternList, &patternCount, &patternCapacity, tempPattern);
        printf("The word pattern is: %s\n", patternList[0].pat);
    }
    
    // Step VIII: Heap deallocation
    for (int i = 0; i < wordCount; i++) {
        free(wordList[i]);
    }
    free(wordList);
    free(patternList[0].pat);
    for (int i = 1; i < patternCount; i++) {
        free(patternList[i].pat);
    }
    free(tempPattern.pat);
    free(patternList);
    
    return 0;
}
