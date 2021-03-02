# Work is done in mostFreqWords.c and wordTree.c

## mostFreqWords.c (Main Program)
1. Takes one/two command line arguments (./mostFreqWords numWords File)
    - The first optional argument gives the number of words to be output
    - The second argument gives the name of a Project Gutenberg text file
    - If the numWords argument is not given, the default value of 10 is used

2. Reads text from the file, and computes word (stem) frequencies

3. Prints a list of the top N most frequent words, from most frequent to least frequent
    - If word frequencies are the same, then lexicographical order is used

## wordTree.c
- Implements an efficient word dictionary data type based on an AVL Binary Search Tree
