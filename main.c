#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"

//Demo hash functions
unsigned long hash_str(char* str){
    unsigned long hash = 5831;
    int c;
    while((c= *str++)){
        hash = ((hash<<5) + hash) + c;
    }
    return hash;
}
int main(){
    
    //Configuring the table
    HashTableConfig* config = (HashTableConfig*)malloc(sizeof(HashTableConfig));
    config->free_key = NULL;
    config->hash_function  = (unsigned long (*)(void*))hash_str;
    config->free_value = NULL;
    config->key_compare = NULL;

    HashTable* ht = ht_create(100, config);

    ht_insert(ht, "KEY", "VALUE");

    return 0;
}

