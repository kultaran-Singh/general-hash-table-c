#include <stdio.h>
#include <stdlib.h>
#include "hash_table.h"

int main(){
    
    HashTableConfig* config = (HashTableConfig*)malloc(sizeof(HashTableConfig));
    config->free_key = NULL;
    config->hash_function = NULL;
    config->free_value = NULL;
    config->key_compare = NULL;

    HashTable* ht = ht_create(100, config);

    return 0;
}