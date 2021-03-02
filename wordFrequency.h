// wordFrequency.h --> definition of (word,freq) type

#ifndef WORD_H
#define WORD_H

typedef struct _WFreq {
   char  *word;  // Word buffer (dynamically allocated)
   int    freq;  // Count of number of occurrences
} WFreq;

#endif
