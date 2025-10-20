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
    HashTableConfig* config;
    unsigned long (*hash_function)(void* key);
    bool (*key_compare)(void* key1, void* key2);
    void (*free_key)(void* key);
    void (*free_value)(void* value);

}HashTable;

//Lifecycle Functions
HashTable* ht_create(int num_buckets, HashTableConfig* config){

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
    new_ht->config = config;
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

void ht_double_size(HashTable* ht){
    //This function is used to double the size of the hashtable, it should be called during insertion

    //First it creates a backup array old_buckets, then it copies all the elements of ht->buckets into 
    //old array, then it clears the ht->buckets array and resizes it, then it rehashes the elements from
    //old buckets array to the newly resized ht->buckets array

    size_t old_size = ht->size; //Storing the old size
    List** old_buckets = calloc(ht->num_buckets, sizeof(List*));

    //This loop Copies all elements from the num_buckets into old_buckets
    for(int i = 0; i < ht->num_buckets; i++){
        old_buckets[i] = ht->buckets[i];
    }
    
    //Cleaning the entire buckets array
    for(int i = 0; i < ht->num_buckets; i++){
        if(ht->buckets[i] == NULL)
            continue;
        ht->buckets[i] = 0;
    }
    //Doubling the size of the new buckets array
    ht->buckets = realloc(ht->buckets, sizeof(List*) * ht->num_buckets * 2);
    ht->num_buckets = ht->num_buckets * 2;
    ht->size = 0;
    
    //Initializing the new elements
    ht_init(ht);

    for(int i = 0; i < old_size; i++){
        list_for_each(old_buckets[i], ht_rehash, ht); //Stores key and value, rehashes key, gets index, inserts it into new table, returns nothing.
    }
    
    return;
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
    if(ht->size >= (ht->num_buckets / 2)){
        ht_double_size(ht);
        return;
    }
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
void* get_value(void* element){
    Entry* entry = element;
    return entry->value;
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

 //Stores key and value, rehashes key, gets index, inserts it into new table, returns nothing.
void ht_rehash(void* element, void* hash_table){
    HashTable* ht = (HashTable*)hash_table;
    void* key = get_key(element);
    void* value = get_value(element);
    ht_insert(ht, key, value);
    return;
}