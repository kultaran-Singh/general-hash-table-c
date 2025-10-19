#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct Node Node;
typedef struct List List;

//List Functions
List* list_create(void (*free_data)(void* data));
bool list_destroy(List* l);

//Insertion Functions
int list_push_back(List* l, void* object);
int list_push_front(List* l, void* object);
int list_push_at_index(List* l, void* object, int index);

//Deletion Functions
void* list_pop_back(List* l);
void* list_pop_front(List* l);
void* list_pop_at_index(List* l, int index);
void* list_pop_by_key(List* l, void* key, void* (*get_key)(void* element), bool (*compare_key)(void* key1, void* key2));

//Lookup Functions
void* get_by_index(List* l, int index);
void* get_by_key(List* l, void* key, void* (*get_key)(void* element), bool (*compare_key)(void* key1, void* key2)); 

//General Functions
size_t get_list_size(List* l);
int list_for_each(List* l, void (*operation)(void* element, void* contest), void* context);

#endif
