#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct Node node;
typedef struct List list;

//List Functions
list* list_create(void (*free_data)(void* data));
bool list_destroy(list* l);

//Insertion Functions
void list_push_back(list* l, void* object);
void list_push_front(list* l, void* object);
void list_push_at_index(list* l, void* object, int index);

//Deletion Functions
void* list_pop_back(list* l);
void* list_pop_front(list* l);
void* list_pop_at_index(list* l, int index);

//Lookup Functions
void* get_item_at_index(list* l, int index);

//General Functions
size_t get_list_size(list* l);
void list_for_each(list* l, void (*operation)(void* data));

#endif
