#include<stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include <linked_list.h>

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
void list_push_back(List* l, void* object){
    
    Node* new_node = (Node*)malloc(sizeof(node));
    if(new_node == NULL){
        printf("ERROR: MEMORY ALLOCATION FAILED.\n");
        return;
    }

    new_node->element = object;
    new_node->next = NULL;

    if(l->head == NULL){
        l->head = new_node;
        l->tail = new_node;
        l->size++;
        return;
    }
    Node* current = l-> head;
    while(current->next != NULL){
        current = current->next;
    }
    current->next = new_node;
    l->tail = new_node;
    l->size++;
    return;

}

void list_push_front(List* l, void* object){

    if(l == NULL){
        printf("ERROR: List does not exist\n");
        return;
    }
    node* current = l->head;
    node* new_node = (node*)malloc(sizeof(node));
    if(current == NULL){
        current = new_node;
        current->element = object;
        current->next = NULL;
        l->size++;
        return;
    }
    
    if(new_node == NULL){
        printf("ERROR: MEMORY ALLOCATION FAILED.\n");
        return;
    }
    new_node->next = current;
    new_node->element = object;
    l->head = new_node;
    l->size++;

}

void list_push_at_index(List* l, void* object, int index){
    
    if(l == NULL){
        printf("ERROR: List does not exist\n");
        return;
    }

    if(index > l->size){
        printf("ERROR: Index out of bounds.\n");
        return;
    }
    else if(index == 0){
        list_push_front(l, object);
        return;
    }
    else if(index == l->size - 1){
        list_push_back(l, object);
        return;
    }
    int current_index = 0;
    Node* current_node = l->head;
    while(current_index < index - 1){
        current_node = current_node->next;
        current_index++;
    }
    Node* new_node = (Node*)malloc(sizeof(node));
    if(new_node == NULL){
        printf("ERROR: MEMORY ALLOCATION FAILED.\n");
        return;
    }
    
    new_node->next = current_node->next;
    new_node->element = object;
    current_node->next = new_node;
    
    l->size++;
    return;
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

//Lookup Functions
void* get_item_at_index(List*l, int index){
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

//General Functions
size_t get_list_size(List* l){
    return l->size;
}

void list_for_each(List* l, void (*operation)(void* data)){
    if(l == NULL){
        printf("ERROR: List does not exist\n");
        return;
    }
    if(l->head == NULL){
        printf("ERROR: List is empty\n");
        return;
    }
    Node* current_node = l->head;
    if(operation == NULL){
        printf("No Operation Provided\n");
        return;
    }
    while(current_node != NULL){
        operation(current_node->element);
        current_node = current_node->next;
    }
    return;
}