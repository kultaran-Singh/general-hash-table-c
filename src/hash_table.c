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
    size_t size;
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
    new_ht->num_buckets = num_buckets;
    new_ht->size = 0;

    //Assigning memory
    new_ht->buckets = calloc(num_buckets, sizeof(List*));
    if(new_ht->buckets == NULL){
        printf("ERROR: Memory allocation Failed");
        return NULL;
    }
    
    //Initializing array
    ht_init(new_ht);
    
    
    return new_ht;
}

void ht_init(HashTable* ht){
    for(int i = 0; i < ht->num_buckets; i++){
        ht->buckets[i] = list_create(NULL);
        if(ht->buckets[i] == NULL){
            printf("ERROR: Failed to create lists");
            continue;
        }
    }
    return;
}

bool ht_destroy(HashTable* ht){
    for(int i = 0; i < ht->num_buckets; i++){
        if(ht->buckets[i] == NULL)
            continue;
        list_for_each(ht->buckets[i], free_entry, ht);
        list_destroy(ht->buckets[i]);
    }
    free(ht->buckets);
    free(ht);
    ht = NULL;
    return true;
}

//Generic Functions
void ht_insert(HashTable* ht, void* key, void* value){
    
    int index = ht->hash_function(key) % ht->num_buckets;
    Entry* new_entry = (Entry*)malloc(sizeof(Entry));
    if(new_entry == NULL){
        printf("ERROR: Couldn't assign memory to new entry");
        return;
    }
    new_entry->key = key;
    new_entry->value = value;
    
    //Handles collision
    if(ht->buckets[index] == NULL){
        printf("ERROR");
        return;
    }
    list_push_back(ht->buckets[index], new_entry);
    ht->size++;
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

bool ht_remove(HashTable* ht, void* key){

    int index = ht->hash_function(key) % ht->num_buckets;

    list_pop_by_key(ht->buckets[index], key, get_key, ht->key_compare, free_entry, ht);
    ht->size--;
    return true;
}

//Helper Functions
void* get_key(void* element){
    Entry* entry = element;
    return entry->key;
}

void print_entry(void* element, void* context){
    Entry* entry = element;
    char* keyPrint = (char*)entry->key;
    char* valuePrint = (char*)entry->value;
    printf("Key: %s\nValue: %s\n", keyPrint, valuePrint);
    return;
}

void ht_print(HashTable* ht){
    for(int i = 0; i < ht->num_buckets; i++){
        printf("%d: ", i);
        int err = list_for_each(ht->buckets[i], print_entry, NULL);
        if(err == 2){
            printf("List is empty\n");
        }
    }
    return;
}

void free_entry(void* element, void* hash_table){
    HashTable* ht = (HashTable*)hash_table;
    Entry* entry = (Entry*) element;
    if(ht->free_key != NULL)
        ht->free_key(entry->key);
    if(ht->free_value != NULL)
        ht->free_value(entry->value);
    free(entry);
    return;
}

size_t ht_size(HashTable* ht){
    return ht->size;
}