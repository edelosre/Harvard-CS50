// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[N];

//Represent dictionary size
int dictionary_size = 0;

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    node *new_node = NULL;

    int hashvalue;

    // Insert words into hash table
    while (fscanf(file, "%s\n", word) != EOF)
    {
        // TODO
        // Allocate memory each for new word
        new_node = malloc(sizeof(node));

        //Copy every word into node and take hash
        strcpy(new_node->word, word);
        hashvalue = hash(new_node->word);

        //The first element will not have a next value and so it becomes the last element
        if (hashtable[hashvalue] == NULL)
        {
            hashtable[hashvalue] = new_node;
            new_node->next = NULL;

        }
        else
        {

           new_node->next = hashtable[hashvalue];
           hashtable[hashvalue] = new_node;

        }


        //printf("Word is: %s --- Hash is: %i, Next is: %s\n", word, hashvalue, new_node->next->word);
        dictionary_size++;

    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (dictionary_size != 0)
    {
        return dictionary_size;
    }
    else
    {
        return 0;
    }
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO

    //Get hash of word
    int hashvalue = hash(word);

    //Create node
    node *matchnode = hashtable[hashvalue];

    //While the words do not match keep transversing node
    while (matchnode != NULL)
    {

        if(strcasecmp(matchnode->word, word) != 0)
        {
            matchnode = matchnode->next;
        }
        else
        {
            //printf("Word --- %s, Match --- %s\n", word, matchnode->word);
            break;
        }

    }

    if(matchnode != NULL)
    {
        return true;

    }
    else
    {
        return false;
    }

}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO

    int buckets_unloaded = 0;
    //Unload every hash bucket one at a time
    for (int i = 0; i < N; i++)
    {
        node* cursor = hashtable[i];
        while (cursor != NULL)
        {
            node* temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
        buckets_unloaded++;
    }

    //If all buckets were not unloaded, then failed
    if (buckets_unloaded != N)
    {
        return false;
    }
    else
    {
        return true;
    }
}
