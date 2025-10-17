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

}HashTable;

HashTable* ht_create(int num_buckets, const HashTableConfig* config){

    HashTable* new_ht;
    new_ht = (HashTable*)malloc(sizeof(HashTable)); //Initializing the table

    if(new_ht == NULL){
        printf("ERROR: Memory allocation Failed");
        return NULL;
    }

    List* head = list_create(config->free_value);
    new_ht->buckets = (List**)calloc(sizeof(head), num_buckets);//Initializing the array
    new_ht->num_buckets = num_buckets;
    return new_ht;
}