// wordCutter.h --> interface to stemming module

#ifndef WORDCUTTER_H
#define WORDCUTTER_H

// In stem(p,i,j), p is a char pointer, and the string to be stemmed is from
// p[i] to p[j] inclusive. Typically i is zero and j is the offset to the last
// character of a string, (p[j+1] == '\0'). The stemmer adjusts the
// characters p[i] ... p[j] and returns the new end-point of the string, k.
//--------------------------------------------------------------------------
// k+1 = '\0'
// strlen gives length of word
// but remember that array index of length is 1 less than length of word
// cat -> ca
// strlen(cat) = 3
// a[0]-a[2]
// a[0]-a[1]
//--------------------------------------------------------------------------
// Stemming never increases word length, so i <= k <= j.

int stem(char *p, int i, int j);

#endif
