#ifndef hash_table_h
#define hahs_table_h
/*
    Ch2. Hash Table Structure
    Key-value pairs 
*/
typedef struct {
    char* key;
    char* value;
} ht_item;

ht_item *ht_new_item(const char *k, const char *v);
void ht_del_item(ht_item *i);

/* 
    Ch2. Hash Table Structure
    Hash table stores an array of pointers to items.
*/
typedef struct {
    int size;
    int count;
    ht_item** items;
} ht_hash_table;
ht_hash_table *ht_new_hash_table(const int size);
void ht_del_hash_table(ht_hash_table *ht);


void ht_insert(ht_hash_table* ht, const char* key, const char* value);
char* ht_search(ht_hash_table* ht, const char* key);
void ht_delete(ht_hash_table* ht, const char* key);

#define HT_PRIME_1 79
#define HT_PRIME_2 59
#define HT_INITIAL_BASE_SIZE 50

#endif