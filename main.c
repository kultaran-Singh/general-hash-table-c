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

void free_str(char* str){
    free(str);
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
    config->free_key = (void (*)(void*))free_str;
    config->hash_function  = (unsigned long (*)(void*))hash_str;
    config->free_value = (void (*)(void*))free_str;
    config->key_compare = (bool (*)(void* ,void* ))compare_str_key;

    HashTable* ht = ht_create(10, config);
    if(ht == NULL){
        printf("ERROR: Failed to create hash table\n");
        return 1;
    }

    ht_insert(ht, "Vansh", "Worker");
    ht_insert(ht, "Dev", "Student");
    ht_insert(ht, "Amit", "Intern");
    ht_insert(ht, "Rez", "Designer");
    ht_insert(ht, "Shiv", "Coder");
    ht_insert(ht, "Ousher", "Business");
    print_ht(ht);
    
    return 0;
}

