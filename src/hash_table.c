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
    bool (*key_compare)(void* key1, void* key2);
    void (*free_key)(void* key);
    void (*free_value)(void* value);

}HashTable;

//Lifecycle Functions
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
    new_ht->buckets = calloc(num_buckets, sizeof(List*));
    if(new_ht->buckets == NULL){
        printf("ERROR: Memory allocation Failed");
        return NULL;
    }
    init_ht(new_ht);
    new_ht->num_buckets = num_buckets;

    return new_ht;
}
void init_ht(HashTable* ht){
    for(int i = 0; i < ht->num_buckets; i++){
        ht->buckets[i] = list_create(NULL);
        if(ht->buckets[i] == NULL){
            printf("ERROR: Failed to create lists");
            return;
        }
    }
    return;
}

//Generic Functions
void ht_insert(HashTable* ht, void* key, void* value){
    
    int index = ht->hash_function(key) % ht->num_buckets;
    Entry* new_entry = (Entry*)malloc(sizeof(Entry));
    new_entry->key = key;
    new_entry->value = value;
    
    //Handles collision
    if(ht->buckets[index] != NULL){
        list_push_back(ht->buckets[index], new_entry);
        return;
    }
    ht->buckets[index] = list_create(ht->free_value);
    list_push_back(ht->buckets[index], new_entry);
    return;
}

void* ht_get(HashTable* ht, void* key){

    int index = ht->hash_function(key) % ht->num_buckets;
    if(ht->buckets[index] == NULL){
        printf("No entry with this key");
        return NULL;
    }
    Entry* entry = get_by_key(ht->buckets[index], key, get_key, ht->key_compare);
    return entry;
}

/*bool ht_remove(HashTable* ht, void* key){

    int index = ht->hash_function(key) % ht->num_buckets;
    if(ht->buckets[index] == NULL){
        printf("No entry with this key");
        return NULL;
    }
    Entry* entry = get_by_key(ht->buckets[index], key, compare);
    ht->free_key(entry->key);
    ht->free_value(entry->value);
}*/

//Helper Functions
void* get_key(void* element){
    Entry* entry = element;
    return entry->key;
}

void print_entry(void* element){
    Entry* entry = element;
    char* keyPrint = (char*)entry->key;
    char* valuePrint = (char*)entry->value;
    printf("Key: %s\nValue: %s\n", keyPrint, valuePrint);
    return;
}

void print_ht(HashTable* ht){
    for(int i = 0; i < ht->num_buckets; i++){
        if(ht->buckets[i] == NULL){
            printf("%d. 0\n", i);
            continue;
        }
        else{
            printf("%d: ", i);
            list_for_each(ht->buckets[i], print_entry);
        }
    }
    return;
}