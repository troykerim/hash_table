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

/*
    CH4
    Collision handling
    Open addresssing with double hasing to perform collision resolution
*/
static int ht_get_hash(const char* s, const int num_buckets, const int attempt)
{
    const int hash_a = ht_hash(s, HT_PRIME_1, num_buckets);
    const int hash_b = ht_hash(s, HT_PRIME_2, num_buckets);
    return (hash_a + (attempt * (hash_b + 1))) % num_buckets;
}
/*
    Ch5
    Insert a new key value pair.
    Iterate through indexes until we find an empty bucket.
    Insert the tiem into that bucket and increment the hash table's count attribute
*/
void ht_insert(ht_hash_table* ht, const char* key, const char* value)
{
    ht_item* item = ht_new_item(key, value);
    int index = ht_get_hash(item->key, ht->size, 0);
    ht_item* cur_item = ht->items[index];
    int i = 1;
    while(cur_item != NULL)
    {
        index = ht_get_hash(item->key, ht->size, i);
        cur_item = ht->items[index];
        i++;
    }
    ht->items[index] = item;
    ht->count++;
}
/*
    CH5
    Searching through each iteration of the loop
    check whether the item's key matches the key we're searching for. 
    If it does, return the item's value
*/
char* ht_search(ht_hash_table* ht, const char* key)
{
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;
    while(item != NULL)
    {
        if(strcmp(item->key, key) == 0)
        {
            return item->value;
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
    return NULL;
}
/*
    CH5
    Mark a deleted item as deleted
    Replace the global pointer to an item
*/
static ht_item HT_DETEDLED_ITEM = {NULL, NULL};
void ht_delete(ht_hash_table* ht, const char* key)
{
    int index = ht_get_hash(key, ht->size, 0);
    ht_item* item = ht->items[index];
    int i = 1;
    while(item != NULL)
    {
        if(item != &HT_DETEDLED_ITEM)
        {
            if(strcmp(item->key, key) == 0)
            {
                ht_del_item(item);
                ht->items[index] = &HT_DETEDLED_ITEM;
            }
        }
        index = ht_get_hash(key, ht->size, i);
        item = ht->items[index];
        i++;
    }
    ht->count--;
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
/*
    CH6
*/
static ht_hash_table* ht_new_sized(const int base_size)
{
    ht_hash_table* ht = xmalloc(sizeof(ht_hash_table));
    ht->base_size = base_size;
    ht->size = next_prime(ht->base_size);
    ht->count = 0;
    ht->items = xcalloc((size_t)ht->size, sizeof(ht_item*));
    return ht;
}
ht_hash_table* ht_new()
{
    return ht_new_sized(HT_INTIAL_BASE_SIZE);
}
static void ht_resize(ht_hash_table* ht, const int base_size) {
    if (base_size < HT_INITIAL_BASE_SIZE) {
        return;
    }
    ht_hash_table* new_ht = ht_new_sized(base_size);
    for (int i = 0; i < ht->size; i++) {
        ht_item* item = ht->items[i];
        if (item != NULL && item != &HT_DELETED_ITEM) {
            ht_insert(new_ht, item->key, item->value);
        }
    }

    ht->base_size = new_ht->base_size;
    ht->count = new_ht->count;

    // To delete new_ht, we give it ht's size and items 
    const int tmp_size = ht->size;
    ht->size = new_ht->size;
    new_ht->size = tmp_size;

    ht_item** tmp_items = ht->items;
    ht->items = new_ht->items;
    new_ht->items = tmp_items;

    ht_del_hash_table(new_ht);
}
static void ht_resize_up(ht_hash_table* ht) {
    const int new_size = ht->base_size * 2;
    ht_resize(ht, new_size);
}
static void ht_resize_down(ht_hash_table* ht) {
    const int new_size = ht->base_size / 2;
    ht_resize(ht, new_size);
}


int main()
{
    ht_hash_table* ht = ht_new();
    ht_del_hash_table(ht);
}