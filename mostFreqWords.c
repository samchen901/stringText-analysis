// Usage: ./mostFreqWords [Nwords] File

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include "wordTree.h"
#include "wordFrequency.h"
#include "wordCutter.h"

#define STOPWORDS "stopwords"
#define MAXLINE 1000
#define MAXWORD 100

#define STARTING "*** START OF"
#define ENDING   "*** END OF"

#define isWordChar(c) (isalnum(c) || (c) == '\'' || (c) == '-')

int main(int argc, char *argv[]) {
    	
    char *fileName;    // Name of file containing book text
    int   nWords;      // Number of top frequency words to show
    
    //--------------------------------------------------------------------------
    // Process command-line args
    switch (argc) {
    case 2:
        nWords = 10;
        fileName = argv[1];
        break;
    case 3:
        nWords = atoi(argv[1]);
        if (nWords < 10) nWords = 10;
        fileName = argv[2];
        break;
    default:
        fprintf(stderr,"Usage: %s [Nwords] File\n", argv[0]);
        exit(EXIT_FAILURE);
	}
    //--------------------------------------------------------------------------
	// Create stopword dictionary
    Dict stopwords = newDict();

    // Open stopwords file
    FILE *stopwordsFile = fopen("stopwords", "r");
    if (stopwordsFile == NULL) {
        fprintf(stderr, "Can't open stopwords\n");
        exit(EXIT_FAILURE);
    }

    char word1[MAXWORD];
    // Scan in stopwords into stopwords dictionary
    while(fscanf(stopwordsFile, "%s", word1) == 1) {
        DictInsert(stopwords, word1);
    }

    fclose(stopwordsFile);
    //--------------------------------------------------------------------------
    // Create dictionary for File to be read
    Dict bookDict = newDict();

    // Open File for reading
    FILE *inputFile = fopen(fileName, "r");
    if (inputFile == NULL) {
        fprintf(stderr, "Can't open %s\n",fileName);
        exit(EXIT_FAILURE);
    }

    char openingLine[] = "*** START OF";
	char closingLine[] = "*** END OF";
	char line[MAXLINE];
    int foundOpening = 0;
    char delimiters[] = " \e\a\b\f\n\r\t\v\\\?\"`~!@#$^&*()_+=[]{}|;:,./<>%";
	
    // Scan File and Skip to line containing "*** START OF"
    while (fgets(line, MAXLINE, inputFile) != NULL) { 
		if (memcmp(openingLine, line, 12) == 0) {
            foundOpening = 1;
            break;
        }
    }
    
    // If cannot find line containing "*** START OF"
    if (foundOpening == 0) {
        fprintf(stderr, "Not a Project Gutenberg book\n");
        exit(EXIT_FAILURE);
    }
    
    // While not EOF and not reached line containing "*** END OF"
    // Scan file and extract words to be put into bookDict dictionary
    while (fgets(line, MAXLINE, inputFile) != NULL) {
        // Loop to find "*** END OF"
		if (memcmp(closingLine, line, 10) == 0) {
            break;
        }
        
        int i = 0;
        char *linePtr;

        // Extract individual words from a line 
        linePtr = strtok(line, delimiters);
        while (linePtr != NULL) {
            strcpy(word1, linePtr);
            // Reject single character "words"
            if (strlen(word1) <= 1) {
                linePtr = strtok(NULL, delimiters);
                continue;
            }
            // Ensure Word is all lowercase
            for (i = 0; word1[i] != '\0'; i++) {
                word1[i] = tolower(word1[i]);
            }
            // Make sure Word is not a Stopword
            if (DictFind(stopwords, word1) == NULL) {
                // Apply stemmer to Word
                int k = stem(word1, 0, strlen(word1)-1);
                word1[k+1] = '\0';
                // Insert word into dictionary
                DictInsert(bookDict, word1);
            }
            linePtr = strtok(NULL, delimiters);
        }
    }
    fclose(inputFile);
    //--------------------------------------------------------------------------
    // Compute and display the topN words
    // Array of top N (word,freq) pairs (Dynamically allocated)
    WFreq *results = malloc(sizeof(WFreq) * nWords);	
    int lengthResults = findTopN(bookDict, results, nWords);
    for (int i = lengthResults-1; i >= 0; i--) {
        printf("%7d %s\n", results[i].freq, results[i].word);
    }

    return EXIT_SUCCESS;
}