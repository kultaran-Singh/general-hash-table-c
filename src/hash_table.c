#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"
#include "linked_list.h"

typedef struct Entry{
    void* key;
    void* value;
}Entry;

typedef struct HashTable{
    List** buckets; //Array of linked list heads
    int num_buckets;
    unsigned long (*hash_function)(void* key);
    int (*key_compare)(void* key1, void* key2);
    void (*free_key)(void* key);
    void (*free_value)(void* value);

}HashTable;

HashTable* ht_create(int num_buckets, const HashTableConfig* config){

    HashTable* new_ht;
    new_ht = (HashTable*)malloc(sizeof(HashTable)); //Initializing the table

    if(new_ht == NULL){
        printf("ERROR: Memory allocation Failed");
        return NULL;
    }

    //Configuring the table
    new_ht->hash_function = config->hash_function;
    new_ht->key_compare = config->key_compare;
    new_ht->free_key = config->free_key;
    new_ht->free_value = config->free_value;

    //Initializing the array
    new_ht->buckets = (List**)calloc(sizeof(list_create), num_buckets);
    new_ht->num_buckets = num_buckets;

    return new_ht;
}

void ht_insert(HashTable* ht, void* key, void* value){

    int index = ht->hash_function(key) % ht->num_buckets;
    if(ht->buckets[index] != NULL){
        list_push_back(ht->buckets[index], value);
        return;
    }
    ht->buckets[index] = list_create(ht->free_value);
    list_push_front(ht->buckets[index], value);
    return;
}