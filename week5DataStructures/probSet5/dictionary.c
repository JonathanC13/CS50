// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 25875 + 1;

int numOfWords = 0;

// const unsigned int hashNumOfChars = 3;

// Hash table
node *table[N];

unsigned int hash(const char *word);

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // get hash index of word. O(1) since uses a calculation to immediately access the index
    int index = hash(word);
    node *iter = table[index];
    while (iter != NULL)
    {
        if (strcasecmp(word, iter->word) == 0)
        {
            return true;
        }
        iter = iter->next;
    }
    return false;

    /*
    iterate linked list with for
    for (node *iter = table[index]; iter->next != NULL; iter = iter->next)
    */
}

void countElemsInBuckets(void);

// Hashes word to a number
/*
1st: index += (toupper(word[i]) - 'A') for the first 3 chars, 0 1 2
    Res: not distributed enough
2nd: sum of all chars (each - 'A')
3rd: sum all the chars but multiply the char location to weight it.
    So that abc value != cab
    unweighted; 3 and 3
    weighted;   8 and 3
*/
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    int index = 0;
    // for (int i = 0, len = (strlen(word) < hashNumOfChars) ? strlen(word) : hashNumOfChars; i < len; i++)
    for (int i = 0, len = strlen(word); i < len; i++)
    {
        if (word[i] == '\'')
        {
            continue;
        }
        index += (toupper(word[i]) - 'A') * (i + 1);
    }
    return index;
}

void calcN(void)
{
    int Nmax = 0;
    char s[45] = "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz";
    for (int i = 0; i < 45; i++)
    {
        Nmax += (toupper(s[i]) - 'A') * (i + 1);
    }
    printf("N = %i\n", Nmax);
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // calcN();
    // return false;
    // TODO
    // Open the dictionary file
    FILE *in = fopen(dictionary, "r");
    if (in == NULL)
    {
        printf("File failure.\n");
        return false;
    }

    char dictWord[LENGTH + 1];

    while (fscanf(in, "%s", dictWord) != EOF)
    {
        // printf("%s\n", dictWord);
        node *wordNode = malloc(sizeof(node));
        if (wordNode == NULL)
        {
            printf("malloc node failure.\n");
            unload(); // need to unload any previously allocated nodes
            fclose(in);
            return false;
        }

        strcpy(wordNode->word, dictWord); // Copy word into node
        wordNode->next = NULL;            // Do not know next yet.

        int index = hash(dictWord); // with the string, return an index that specifies which bucket to store it in the hash
                                    // table.

        // insert the node into the index of the hash table. Just put it at the start of the linked list of the index.
        if (table[index] != NULL)
        {
            wordNode->next = table[index];
            table[index] = wordNode;
        }
        else
        {
            table[index] = wordNode;
        }

        numOfWords++;
    }

    // countElemsInBuckets();

    fclose(in);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    /*
    int count = 0;
    node *iter = NULL;
    for (int i = 0; i < N; i++)
    {
        if (table[i] == NULL)
        {
            continue;
        }
        else
        {
            iter = table[i];
            while (iter != NULL)
            {
                count++;
                iter = iter->next;
            }
        }
    }
    return count;
    */
   return numOfWords;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        if (table[i] == NULL)
        {
            continue;
        }
        else
        {
            // iterate over all nodes at this index and free.
            node *iter = table[i];
            while (iter != NULL)
            {
                node *temp = iter;
                iter = iter->next;
                free(temp);
            }

            // with for
            /*
            for (node *iter = table[i]; iter != NULL; )
            {
                node *temp = iter;
                iter = iter->next;
                free(temp);
            }
            */
        }
    }
    return true;
}

void countElemsInBuckets(void)
{
    int count = 0;
    node *iter = NULL;
    //for (int i = 0; i < N; i++)
    for (int i = N - 1; i > (N - 10); i--)
    {
        count = 0;
        printf("Bucket %i:", i);
        if (table[i] == NULL)
        {
            count = 0;
        }
        else
        {
            iter = table[i];
            while (iter != NULL)
            {
                count++;
                iter = iter->next;
            }
        }
        printf("%i\n", count);
    }
}

/*
./speller dictionaries/small text
valgrind ./speller dictionaries/small text
help50 valgrind ./speller dictionaries/small text

You could therefore run your program on some text in one window, as with the below.

./speller texts/lalaland.txt
---
WORDS MISSPELLED:     955
WORDS IN DICTIONARY:  143092
WORDS IN TEXT:        17756
TIME IN load:         0.02
TIME IN check:        0.04
TIME IN size:         0.00
TIME IN unload:       0.01
TIME IN TOTAL:        0.08
---
Conclusion is if you want to brute force performance you would use a lot of memory
    to have each string to have a unique hash value. A better hash function will reduce 'TIME IN check'

And you could then run the staff’s solution on the same text in another window, as with the below.

./speller50 texts/lalaland.txt
---
WORDS MISSPELLED:     955
WORDS IN DICTIONARY:  143092
WORDS IN TEXT:        17756
TIME IN load:         0.02
TIME IN check:        0.01
TIME IN size:         0.00
TIME IN unload:       0.01
TIME IN TOTAL:        0.04
---

And you could then compare the windows visually side by side. That could get tedious quickly, though.
So you might instead want to “redirect” your program’s output to a file, as with the below.

./speller texts/lalaland.txt > student.txt
./speller50 texts/lalaland.txt > staff.txt

You can then compare both files side by side in the same window with a program like diff, as with the below.

diff -y student.txt staff.txt

Alternatively, to save time, you could just compare your program’s output (assuming you redirected it to, e.g., student.txt)
against one of the answer keys without running the staff’s solution, as with the below.

diff -y student.txt keys/lalaland.txt
*/

/*
To test your code less manually (though still not exhaustively), you may also execute the below.

check50 cs50/problems/2023/x/speller

Results for cs50/problems/2023/x/speller generated by check50 v3.3.8
:) dictionary.c exists
:) speller compiles
:) handles most basic words properly
:) handles min length (1-char) words
:) handles max length (45-char) words
:) handles words with apostrophes properly
:) spell-checking is case-insensitive
:) handles substrings properly
:) program is free of memory errors
---

Note that check50 will also check for memory leaks, so be sure you’ve run valgrind as well.

style50

Execute the below to evaluate the style of your code using style50.

style50 dictionary.c
*/