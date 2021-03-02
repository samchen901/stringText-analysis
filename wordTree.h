#ifndef WORDTREE_H
#define WORDTREE_H

#include "wordFrequency.h"

typedef struct _DictRep *Dict;

// Create new empty Dictionary
Dict newDict();

// Insert new word into Dictionary
// Return pointer to (w,f) pair for Word
WFreq *DictInsert(Dict d, char *w);

// Find Word in Dictionary
// Return pointer to (w,f) pair for Word
WFreq *DictFind(Dict d, char *w);

// Find top N frequently occurring words in Dict
// Input: Dictionary, array of WFreqs, size of array
// Returns: #WFreqs in array, modified array
int findTopN(Dict d, WFreq *wfs, int n);

// Print a dictionary
void showDict(Dict d);

#endif
