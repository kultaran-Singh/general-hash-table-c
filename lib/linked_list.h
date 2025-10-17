#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct Node Node;
typedef struct List List;

//List Functions
List* list_create(void (*free_data)(void* data));
bool list_destroy(List* l);

//Insertion Functions
void list_push_back(List* l, void* object);
void list_push_front(List* l, void* object);
void list_push_at_index(List* l, void* object, int index);

//Deletion Functions
void* list_pop_back(List* l);
void* list_pop_front(List* l);
void* list_pop_at_index(List* l, int index);

//Lookup Functions
void* get_by_index(List* l, int index);
void* get_by_key(List* l,void* key, void* (*operation)(void* data));

//General Functions
size_t get_list_size(List* l);
void list_for_each(List* l, void (*operation)(void* data));

#endif
