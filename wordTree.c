// wordTree ADT using AVL BST

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "wordTree.h"
#include "wordFrequency.h"

typedef struct _DictNode *Link;

typedef struct  _DictNode {
    WFreq  data;
    Link   left;
    Link   right;
    int    height; 
} DictNode;

struct _DictRep {
    Link root; // Root node of a tree
};

//------------------------------------------------------------------------------
// Function Prototypes
Link newNode(char *w);
Dict newDict();
WFreq *DictInsert(Dict d, char *w);
Link insertAVL(Link node, char *w);
int height(Link node);
Link rotateRight(Link node);
Link rotateLeft(Link node);
int max(int a, int b);
WFreq *DictFind(Dict d, char *w);
int findTopN(Dict d, WFreq *wfs, int n);
void showDict(Dict d);
void showBSTreeNode (Link t);
void BSTreeInfix (Link t);
int insertArray(Link node, WFreq *wfs, int *length, int maxLength);
void BSTreeSearch(Link node, WFreq *wfs, int *length, int maxLength);
int cmpFunc(const void *a, const void *b);
//------------------------------------------------------------------------------
// Function Implementation
//------------------------------------------------------------------------------
// Create Nodes
Link newNode(char *w) {
    Link newNode = malloc(sizeof(struct _DictNode));
    if (newNode == NULL) {
        free(newNode);
        exit(EXIT_FAILURE);
    }
    newNode->right = NULL;
    newNode->left = NULL;
    newNode->height = 0; 
    newNode->data.freq = 1; // Maybe might be 1
	char *copy = strdup(w);
    newNode->data.word = copy;
    return newNode;
}
//------------------------------------------------------------------------------
// Create new empty Dictionary
Dict newDict() {
    // Create the Dictionary Structure Representation
    Dict newEmptyDict = malloc(sizeof(struct _DictRep));
    if (newEmptyDict == NULL) {
        // Couldnt allocate storage for it
        free(newEmptyDict);
        exit(EXIT_FAILURE);
    }

    newEmptyDict->root = NULL;

    return newEmptyDict;
}
//------------------------------------------------------------------------------
// Insert new word into Dictionary
// Return pointer to the (word,freq) pair for that word
// DictInsert(stopwords, word)
WFreq *DictInsert(Dict d, char *w) {
    // If tree doesn't have root, then add root
    if (d->root == NULL) {
        d->root = newNode(w);
        return &(d->root->data);
    }
    
    // If word is already in dictionary
    // Increment frequency of old word
    WFreq *alreadyWord = DictFind(d,w);
    if (alreadyWord != NULL) { // "Word" is not unique
        alreadyWord->freq++;
        return alreadyWord;
    }
    
    // If word is not in dictionary, then add it in
    d->root = insertAVL(d->root, w);
    WFreq *newNode2 = DictFind(d, w);
    
    return newNode2;
}

// Recursive Insert into AVL Tree
// Assume that root node is already in tree
// Assume that there are no duplicates
// Returns the root node
// Inspired by Jas Pseudocode
Link insertAVL(Link node, char *w) {
    if (node == NULL) {
        return newNode(w);
    } else {
        // Insert recursively
        if (strcmp(w, node->data.word) < 0) {
            node->left = insertAVL(node->left, w);
        } else if (strcmp(w, node->data.word) > 0) {
            node->right = insertAVL(node->right, w);
        }
        // Insertion done
        // Correct the height of the current subtree
        node->height = 1 + max(height(node->left), height(node->right));

        // Rebalance
        int LHeight = height(node->left);
        int RHeight = height(node->right);
        if ((LHeight - RHeight) > 1) {
            if (strcmp(w, node->left->data.word) > 0) {
                node->left = rotateLeft(node->left);
            }
            node = rotateRight(node);
        } else if ((RHeight - LHeight) > 1) {
            if (strcmp(w, node->right->data.word) < 0) {
                node->right = rotateRight(node->right);
            }
            node = rotateLeft(node);
        }

        return node;
    }    
}

// Right Rotation on Node
Link rotateRight(Link node) {
    if (node == NULL || node->left == NULL) {
        return node;
    }
    Link node2 = node->left;
    node->left = node2->right;
    node2->right = node;
    node->height = 1 + max(height(node->left), height(node->right));
    node2->height = 1 + max(height(node2->left), height(node2->right));
    return node2;
}

// Left Rotation on Node
Link rotateLeft(Link node) {
    if (node == NULL || node->right == NULL) {
        return node;
    }
    Link node2 = node->right;
    node->right = node2->left;
    node2->left = node;
    node->height = 1 + max(height(node->left), height(node->right));
    node2->height = 1 + max(height(node2->left), height(node2->right));
    return node2;
}

// Return the max number when given 2 numbers
int max(int a, int b) {
    if (a > b) {
        return a;
    } else {
        return b;
    }
}

// Return the height of the node
int height(Link node) {
    if (node == NULL) {
        return -1;
    } else {
        return node->height;
    }
}
//------------------------------------------------------------------------------
// Find Word in Dictionary
// Return pointer to (word,freq) pair if found, otherwise return NULL
// Return value is non-NULL if word is NOT UNIQUE
// Return value is NULL if word is UNIQUE
WFreq *DictFind(Dict d, char *w) {
    if (d->root == NULL) {
        return NULL;
    }
    Link curr = d->root;
    while (curr != NULL) {
        if (strcmp(w, curr->data.word) < 0) {
            curr = curr->left;
        } else if (strcmp(w, curr->data.word) > 0) {
            curr = curr->right;
        } else if (strcmp(w, curr->data.word) == 0) {
            return &(curr->data);
        }
    }
    return NULL;
}
//------------------------------------------------------------------------------
// Find top N frequently occurring words in Dict
// To do findTopN: traverse the whole tree and maintain, in the array, 
// an ordered sequence of the most frequent words that you've seen so far. 
// If you see another more frequent one, put it in the appropriate spot in the 
// array and push the less frequent ones down.
// Input: Dictionary, array of WFreqs, size of array
// Returns: #WFreqs in array, modified array
int findTopN(Dict d, WFreq *wfs, int n) {
    int length = 0;
    int maxLength = n;
    Link t = d->root;
    BSTreeSearch(t, wfs, &length, maxLength);
    return length;
}

int insertArray(Link node, WFreq *wfs, int *length, int maxLength) {
    // Insert into array until full
    // Dont worry about order
    // Order is taken care of by qsort
    if (*length < maxLength) {
        wfs[*length].freq = node->data.freq;
        wfs[*length].word = strdup(node->data.word);
        *length = *length + 1;
        qsort(wfs, *length, sizeof(struct _WFreq), cmpFunc);
        return *length;
    }
    // Array is now full
    // If new node frequency is greater than smallest node's frequency in array
    // then insert new node into array and call qsort
    if (node->data.freq > wfs[0].freq) {
        wfs[0].freq = node->data.freq;
        wfs[0].word = strdup(node->data.word);
        qsort(wfs, maxLength, sizeof(struct _WFreq), cmpFunc);
    }
    return *length;
}

// Recursively go through whole tree
void BSTreeSearch(Link node, WFreq *wfs, int *length, int maxLength) {
    if (node == NULL) {
        return;
    }
    BSTreeSearch(node->left, wfs, length, maxLength);
    BSTreeSearch(node->right, wfs, length, maxLength);
    insertArray(node, wfs, length, maxLength);
    return;
}

// qsort(wfs, length, sizeof(struct _WFreq), cmpFunc);
// Remember that qsort sorts in ASCENDING order
// If want qsort to sort in descending order then swap < and > around
// or negate (multiply by -1) the return values.
// array[0] will contain lowest frequency word
// array[maxLength] will contain highest frequency word
int cmpFunc(const void *a, const void *b) {
    const struct _WFreq *a1 = a;
    const struct _WFreq *b1 = b;

    // Sort by frequency order first
    if (a1->freq < b1->freq) {
        return -1;
    } else if (a1->freq > b1->freq) {
        return 1;
    } 

    // If frequencies are the same, then sort by alphabetical order    
    int comp = strcmp(a1->word, b1->word);
    return -1*comp;
    
}
//------------------------------------------------------------------------------
// Print a dictionary
void showDict(Dict d) {
    Link t = d->root;
	BSTreeInfix(t);
    return;
}

// Display BSTree
void showBSTreeNode (Link node) {
	if (node == NULL) {
		return;
	}
	printf("%s ", node->data.word);
    return;
}

// Print values in "Infix order"
void BSTreeInfix (Link node) {
	if (node == NULL) {
		return;
    }

	BSTreeInfix(node->left);
	showBSTreeNode(node);
	BSTreeInfix(node->right);
    return;
}
