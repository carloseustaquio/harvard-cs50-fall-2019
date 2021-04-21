// Implements a dictionary's functionality
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdbool.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = (26 * 25) - 1;

// Hash table
node *table[N];

// Number of words in the dictionary
long numWords = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    unsigned int index = hash(word);

    for (node *tmp = table[index]; tmp != NULL; tmp = tmp->next)
    {
        if (strcasecmp(tmp->word, word) == 0)
        {
            return true;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    char letters[26] =
    {
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h',
        'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
        'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'
    };

    int index = 0;

    for (int i = 0; i < 26; i++)
    {
        if (letters[i] == tolower(word[0]))
        {
            index = i * 26;

            if (!word[1])
            {
                return index;
            }

            for (int j = 0; j < 26; j++)
            {
                if (letters[j] == tolower(word[1]))
                {
                    index += j;
                    return index;
                }
            }
        }
    }

    return 0;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");

    if (file == NULL)
    {
        return false;
    }

    char word[LENGTH + 1];

    while (fscanf(file, "%s", word) != EOF)
    {
        node *n = malloc(sizeof(node));
        // n->next = NULL;

        if (n == NULL)
        {
            return false;
        }

        strcpy(n->word, word);

        unsigned int index = hash(word);

        n->next = table[index];
        table[index] = n;

        numWords++;
    }

    // show my table
    // for (int i = 0; i < N; i++)
    // {
    //     if (table[i] != NULL)
    //     {
    //         printf("index %i = ", i);
    //         for (node *tmp = table[i]; tmp != NULL; tmp = tmp->next)
    //         {
    //             printf("%s ", tmp->word);
    //         }
    //         printf("\n");
    //     }
    // }

    fclose(file);
    // TODO
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return numWords;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *tmp = table[i];
        while (table[i] != NULL)
        {
            tmp = tmp->next;
            free(table[i]);
            table[i] = tmp;
        }
    }
    return true;
}
