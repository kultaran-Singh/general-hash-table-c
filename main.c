#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"

//Demo hash functions
//All these functions are for a hash table that stores string keys and string values
unsigned long hash_str(char* str){
    unsigned long hash = 5831;
    int c;
    while((c= *str++)){
        hash = ((hash<<5) + hash) + c;
    }
    return hash;
}

bool compare_str_key(void* key1, void* key2){
    char* key_str1 = (char*) key1;
    char* key_str2 = (char*) key2;
    if(key_str1 == key_str2){
        return true;
    }
    else{
        return false;
    }
}

int main(){
    
    //Configuring the table
    HashTableConfig* config = (HashTableConfig*)malloc(sizeof(HashTableConfig));
    config->free_key = NULL;
    config->free_value = NULL;
    config->hash_function  = (unsigned long (*)(void*))hash_str;
    config->key_compare = (bool (*)(void* ,void* ))compare_str_key;

    HashTable* ht = ht_create(10, config);
    if(ht == NULL){
        printf("ERROR: Failed to create hash table\n");
        return 1;
    }

    //Inserting data into the hashtable
    //NOTE: Keys and Values can be of any type, they are strings in this example
    ht_insert(ht, "Alice Smith", "Software Engineer");
    ht_insert(ht, "Bob Johnson", "Graphic Designer");
    ht_insert(ht, "Charlie Brown", "Project Manager");
    ht_insert(ht, "David Lee", "Data Analyst");
    ht_insert(ht, "Emily White", "Marketing Specialist");
    ht_insert(ht, "Frank Miller", "Accountant");
    ht_insert(ht, "Grace Davis", "HR Coordinator");
    ht_insert(ht, "Henry Wilson", "Electrician");
    ht_insert(ht, "Ivy Chen", "Nurse");
    ht_insert(ht, "Jack Taylor", "Teacher");
    
    //Using ht_get to lookup if a key has a corresponding value, then printing the 
    //entry with print_entry
    print_entry(ht_get(ht, "Ivy Chen"), NULL);
    ht_remove(ht, "Ivy Chen");
    print_entry(ht_get(ht, "Ivy Chen"), NULL);
    printf("-------------------------------\n");
    //Using ht_print to print the whole hashtable
    ht_print(ht);
    
    //Calling ht_destroy to destroy the table and free all memory
    ht_destroy(ht);
    free(config);
    config = NULL;
    return 0;
}

