/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "dictionary.h"

#define TABLE_SIZE 1000

typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// build hash table as a global variable
node* hashtable[TABLE_SIZE];

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // find hash code for word
    unsigned int index = hash(word);
    
    // cursor points to hash's linked list
    node *cursor = hashtable[index];
    
    // check list for word
    while (cursor != NULL)
    {
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }
    
    return false;
}

// used BKDR hash source from http://www.partow.net/programming/hashfunctions
// and https://www.programmingalgorithms.com/algorithm/bkdr-hash?lang=C%2B%2B
// edited with modulo (to avoid overflow and limit hash output to array size)
// and tolower for consistent hashing.

unsigned int hash(const char* str)
{
   unsigned int seed = 131; /* 31 131 1313 13131 131313 etc.. */
   unsigned int hash = 0;
   unsigned int i    = 0;
   unsigned int n    = 0;
   
   // force str lower case for good hashing
   char lowerstr[strlen(str)];
   strcpy(lowerstr, str);
   for (int j = 0, n = strlen(lowerstr); j < n; j++)
   {
       lowerstr[j] = tolower(lowerstr[j]);
   }
   
   // do the hash 
   for (i = 0, n = strlen(lowerstr); i < n; ++i)
   {
      hash = ((hash * seed) + (lowerstr[i])) % TABLE_SIZE;
   }

   return hash;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // open dictionary file
    FILE* dp = fopen(dictionary, "r");
    if (dp == NULL)
    {
        printf("File does not exist.\n");
        return false;
    }
    
    // build hash table (at top as global var)
    // node* hashtable[TABLE_SIZE];
    
    // prepare variables
    char word[LENGTH + 1];
    unsigned int index;
    
    // scan dictionary word by word
    while (fscanf(dp, "%s", word) != EOF)
    {
        // get the index number for the hash table
        index = hash(word);
        
        // create a new node (a pointer to a malloc node sized space, really) and copy the current word into it
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            unload();
            return false;
        }
        strcpy(new_node->word, word);
        
        // insert or chain to linked list at current index
        if (hashtable[index] == NULL)
        {
            hashtable[index] = new_node;
        }
        else
        {
            new_node->next = hashtable[index];
            hashtable[index] = new_node;
        }
    }
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    int words = 0;
    
    // for each hash table row
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        // make cursor, which points to current hash row
        node *cursor = hashtable[i];
    
        // count data in row's linked list
        while (cursor != NULL)
        {
            words++;
            cursor = cursor->next;
        }
    }
    return words;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // for each hash table row
    for (int i = 0; i < TABLE_SIZE; i++)
    {
        // make cursor, which points to current hash row
        node *cursor = hashtable[i];
    
        // free data in row's linked list
        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    return true;
}
