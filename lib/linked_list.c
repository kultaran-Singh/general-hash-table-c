#include<stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "linked_list.h"

typedef struct Node{
    void* element;
    struct Node* next;
}node;

typedef struct List{
    struct Node* head;
    struct Node* tail;
    void (*free_data)(void* data);
    size_t size;
}List;

//List Functions
List* list_create(void (*free_data)(void* data)){
    List* l;
    l = (List*)malloc(sizeof(List));
    
    if(l == NULL){
        printf("ERROR: List Creation Failed");
        return NULL;
    }

    l->head = NULL;
    l->free_data = free_data;
    l->tail = NULL;
    l->size = 0;
    return l;
}

bool list_destroy(List* l){
    if(l == NULL){
        printf("ERROR: List does not exist\n");
        return NULL;
    }
    Node* current_node = l->head;
    Node* next_node;

    while(current_node != NULL){
        next_node = current_node->next;

        if(l->free_data != NULL){
            l->free_data(current_node->element);
        }
        
        free(current_node);
        current_node = next_node;
    }
    free(l);
    return true;
}

//Insertion Functions
int list_push_back(List* l, void* object){
    
    Node* new_node = (Node*)malloc(sizeof(node));
    if(new_node == NULL){
        return 3;
    }

    new_node->element = object;
    new_node->next = NULL;

    if(l->head == NULL){
        l->head = new_node;
        l->tail = new_node;
        l->size++;
        return 0;
    }
    Node* current = l-> head;
    while(current->next != NULL){
        current = current->next;
    }
    current->next = new_node;
    l->tail = new_node;
    l->size++;
    return 0;

}

int list_push_front(List* l, void* object){

    if(l == NULL){
        return 1;
    }
    node* new_node = (node*)malloc(sizeof(node));
    if(new_node == NULL){
        return 3;
    }
    if(l->head == NULL){
        l->head = new_node;
        l->tail = new_node;
        l->size++;
        return 0;
    }
    node* current = l->head;
    
    if(current == NULL){
        current = new_node;
        current->element = object;
        current->next = NULL;
        l->size++;
        return 0;
    }
    

    new_node->next = current;
    new_node->element = object;
    l->head = new_node;
    l->size++;
    return 0;
}

int list_push_at_index(List* l, void* object, int index){
    
    if(l == NULL){
        return 1;
    }

    if(index > l->size){
        return 2;
    }
    else if(index == 0){
        list_push_front(l, object);
        return 0;
    }
    else if(index == l->size - 1){
        list_push_back(l, object);
        return 0;
    }
    int current_index = 0;
    Node* current_node = l->head;
    while(current_index < index - 1){
        current_node = current_node->next;
        current_index++;
    }
    Node* new_node = (Node*)malloc(sizeof(node));
    if(new_node == NULL){
        return 3;
    }
    
    new_node->next = current_node->next;
    new_node->element = object;
    current_node->next = new_node;
    
    l->size++;
    return 0;
}

//Deletion Functions
void* list_pop_back(List* l){

    if(l == NULL){
        printf("ERROR: List does not exist\n");
        return NULL;
    }
    if(l->head == NULL){
        printf("ERROR: List is empty\n");
        return NULL;
    }
    Node* current_node = l->head;
    Node* last_node;

    //Iterating upto the second last node
    while(current_node->next->next != NULL){
        current_node = current_node->next;
    }
    last_node = current_node->next;
    current_node->next = NULL;
    l->tail = current_node;

    //Gets the value of the last node to return it
    void* value = last_node->element;
    free(last_node);
    l->size--;
    return value;
}

void* list_pop_front(List* l){
    if(l == NULL){
        printf("ERROR: List does not exist\n");
        return NULL;
    }
    if(l->head == NULL){
        printf("ERROR: List is empty\n");
        return NULL;
    }
    Node* to_delete = l->head;

    //Getting Value of node to delete
    void* value = to_delete->element;
    //Setting the new head node
    l->head = l->head->next;

    free(to_delete);
    l->size--;
    return value;

}

void* list_pop_at_index(List* l, int index){
    if(l == NULL){
        printf("ERROR: List does not exist\n");
        return NULL;
    }
    if(index > l->size){
        printf("ERROR: Index out of bounds.\n");
        return NULL;
    }
    else if(index == 0){
        return list_pop_front(l);
    }
    else if(index == l->size - 1){
        return list_pop_back(l);
    }

    int current_index = 0;
    Node* current_node = l->head;
    //Iterating upto the node before index to delete
    while(current_index < index - 1){
        current_node = current_node->next;
        current_index++;
    }
    Node* toDelete = current_node->next;
    current_node->next = current_node->next->next;
    void* value = toDelete->element;
    
    free(toDelete);
    l->size--;
    return value;
}

int list_pop_by_key(List* l, 
                      void* key, 
                      void* (*get_key)(void* element), 
                      bool (*compare_key)(void* key1, void* key2), 
                      void (*free_data)(void* data, void* context),
                      void* optional){

    if(l == NULL){
        return 1;
    }
    if(l->head == NULL){
        return 2;
    }
    if(get_key == NULL || compare_key == NULL || free_data == NULL){
        return 4;
    }

    Node* current_node = l->head;
    Node* next_node = current_node->next;

    if(compare_key(get_key(current_node->element), key)){
        //Deleting Head Node 
        Node* toDelete = l->head;
        l->head = l->head->next;
        free_data(toDelete->element, optional);
        free(toDelete);
        return 0;
    }
    while(next_node != NULL){
        if(compare_key(get_key(next_node->element), key)){
            //Delete next node, current_node->next = current_node->next->next

            current_node->next = current_node->next->next;
            free_data(next_node->element, optional);
            free(next_node);
            return 0;
        }
        current_node = current_node->next;
        next_node = next_node->next;
    }
    return 0;
}
//Lookup Functions
void* get_by_index(List*l, int index){
    if(l == NULL){
        printf("ERROR: List does not exist\n");
        return NULL;
    }
    if(index > l->size){
        printf("ERROR: Index out of bounds.\n");
        return NULL;
    }
    else if(index == 0){
        return l->head->element;
    }
    else if(index == l->size - 1){
        return l->tail->element;
    }

    int current_index = 0;
    Node* current_node = l->head;
    //Iterating upto the node before index to get
    while(current_index < index - 1){
        current_node = current_node->next;
        current_index++;
    }
    return current_node->next->element;
}

void* get_by_key(List* l, void* key, void* (*get_key)(void* element), bool (*compare_key)(void* key1, void* key2)){
    //This function is used to search through the list and return elements with a matching "Key" attribute.
    //The first function should get a key from the element and the second function should compare the returned key and
    //compare it to the second key.
    if(l == NULL){
        printf("ERROR: List does not exist\n");
        return NULL;
    }
    if(l->head == NULL){
        printf("ERROR: List is empty\n");
        return NULL;
    }
    if(get_key == NULL || compare_key == NULL){
        printf("ERROR: No Operation Provided\n");
        return NULL;
    }
    Node* current_node = l->head;
    while(current_node != NULL){
        if(compare_key(get_key(current_node->element), key)){
            return current_node->element;
        }
        current_node = current_node->next;
    }
    return NULL;
}

//General Functions
size_t get_list_size(List* l){
    return l->size;
}

int list_for_each(List* l, void (*operation)(void* element, void* context), void* context){
    if(l == NULL){
        return 1;
    }
    if(l->head == NULL){
        return 2;
    }
    if(operation == NULL){
        return 4;
    }
    Node* current_node = l->head;
    while(current_node != NULL){
        if(context == NULL){
            operation(current_node->element, NULL);
            current_node = current_node->next;
        }
        if(context != NULL){
            operation(current_node->element, context);
            current_node = current_node->next;
        }
    }
    return 0;
}
