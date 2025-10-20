#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "linked_list.h"

typedef struct HashTable HashTable;
typedef struct Entry Entry;
typedef struct HashTableConfig{
    unsigned long (*hash_function)(void* key);
    bool (*key_compare)(void* key1, void* key2);
    void (*free_key)(void* key);
    void (*free_value)(void* value);
}HashTableConfig;

//Lifecylce Functions
HashTable* ht_create(int num_buckets, HashTableConfig* config);
void ht_init(HashTable* ht);
bool ht_destroy(HashTable* ht);
void ht_double_size(HashTable* ht);

//Generic Functions
void ht_insert(HashTable* ht, void* key, void* value);
void* ht_get(HashTable* ht, void* key);
bool ht_remove(HashTable* ht, void* key);

//Helper Functions
void* get_key(void* element);
void* get_value(void* element);
void print_entry(void* element, void* context);
void ht_print(HashTable* ht); //For Debugging
void free_entry(void* element, void* ht);
size_t ht_size(HashTable* ht);
void ht_rehash(void* element, void* context);

#endif
