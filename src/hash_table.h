/*
    Ch2. Hash Table Structure
    Key-value pairs 
*/
typedef struct {
    char* key;
    char* value;
} ht_item;

/* 
    Ch2. Hash Table Structure
    Hash table stores an array of pointers to items.
*/
typedef struct {
    int size;
    int count;
    ht_item** items;
} ht_hash_table;

