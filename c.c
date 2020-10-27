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
    list_t* root = new_node(5);
    size_t addresss_value = root;
    list_t* iterator;

    for(int i = 0; i < 5; i++)
    {
        iterator = (list_t*) addresss_value;
        printf("data: %d\n",iterator->data);
        iterator->next = new_node(i);

        addresss_value = iterator->next;
    }

    addresss_value = root;
    while (1)
    {
        iterator = (list_t*) addresss_value;
        if (iterator == NULL)
        {
            break;
        }

        printf("data: %d\n", iterator->data);
        if (iterator->next == NULL)
        {
            break;
        }
        addresss_value = iterator->next;
        
    }

}