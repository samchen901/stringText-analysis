# Work is done in mostFreqWords.c and wordTree.c

## mostFreqWords.c (Main Program)
1. Takes one/two command line arguments (./mostFreqWords numWords fileName)
    - The first optional argument chooses 'k' (the top 'k' most frequent words to be output)
    - The second argument gives the name of a Project Gutenberg text file
    - If the numWords argument is not given, the default value of 10 is used

2. Reads text from the Project Gutenberg text file, performs word stemming and and computes word frequencies (implemented using AVL Binary Search Tree)

3. Prints a list of the top 'k' most frequent words, from most frequent to least frequent
    - If word frequencies are the same, then lexicographical order is used

## wordTree.c
- Implements an efficient word dictionary data structure based on an AVL Binary Search Tree
