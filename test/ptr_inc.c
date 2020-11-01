#include <stdio.h>
#include <stdlib.h>

typedef struct list_struct {
    int data;
    struct list_struct* next;
} list_t;

list_t* new_node(int d)
{
    list_t* c = (list_t*) malloc(sizeof(list_t));
    c->data = d;
    c->next = NULL;

    return c;
}

int main()
{
    
}