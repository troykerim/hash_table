#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

/*
    Ch2
    Initialization fucntion to allocate a chunk of memory at a size of an ht_item
    Saves a copy of string k and v in the new chunk of memory
*/
static ht_item* ht_new_item(const char* k, const char* v) {
    ht_item* i = malloc(sizeof(ht_item));
    i->key = strdup(v);
    i->value = strdup(v);
    return i;
}
/*
    Ch2
    Initializes a new hash table.  The size is fixed at 53, and defines how many items that can be stored. 
    Callow will initialize the array of items, which fills the allocated memory with null bytes, Null indicates that the bucked is empty
*/
ht_hash_table* ht_new()
{
    ht_hash_table* ht = malloc(sizeof(ht_hash_table));
    ht->size = 53;
    ht->count = 0;
    ht->items = calloc((size_t)ht->size, sizeof(ht_item*));
    return ht;
}

/*
    CH2
    Function for deleting ht_items and ht_hash_tables to free memory that was allocated
*/
static void ht_del_item(ht_item* i){
    free(i->key);
    free(i->value);
    free(i);
}
/*
    Ch3
    Take a string as input and return a number between 0 and m (length of bucket)
    Return an even distribution of bucket indexes for an average set of inputs.  
*/
static int ht_hash(const char* s, const int a, const int m)
{
    long hash = 0;
    const int len_s = strlen(s);
    for(int i = 0; i< len_s; i++)
    {
        hash += (long)pow(a, len_s - (i+1)) * s[i];
        hash = hash % m; 
    }
    return (int)hash;
}



void ht_del_hash_table(ht_hash_table* ht)
{
    for(int i = 0; i< ht->size; i++)
    {
        ht_item* item = ht->items[i];
        if(item != NULL)
        {
            ht_del_time(item);
        }
    }
    free(ht->items);
    free(ht);
}

int main()
{
    ht_hash_table* ht = ht_new();
    ht_del_hash_table(ht);
}