#ifndef HASH_TABLE_H
#define HASH_TABLE_H

typedef struct HashTable HashTable;
typedef struct Entry Entry;
typedef struct HashTableConfig{
    unsigned long (*hash_function)(void* key);
    int (*key_compare)(void* key1, void* key2);
    void (*free_key)(void* key);
    void (*free_value)(void* value);
}HashTableConfig;

//Lifecylce Functions
HashTable* ht_create(int num_buckets, const HashTableConfig* config);
bool ht_destroy(HashTable* ht);

//Generic Functions
void ht_insert(HashTable* ht, void* key, void* value);
void* ht_get(HashTable* ht, void* key);
bool ht_remove(HashTable* ht, void* key);

//Helper Functions
void* compare(void* element);

#endif
