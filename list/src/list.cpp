
#include "list.h"

int main ()
{
    List list = {};

    ListCtor (&list, LIST_INITIAL_CAPACITY);

   


    ListDtor (&list);
}


int ListDelete (List* list, node* pos)
{
    if (pos == list->tail)
    {
        pos->prev->next = 0;
        list->tail = pos->prev;
    }
    else if (pos == list->head)
    {
        pos->next->prev = 0;
        list->head = pos->next;
    }
    else
    {
        pos->next->prev = pos->prev;
        pos->prev->next = pos->next;
    }

    free (node);
    node = nullptr;
    
    return elem_to_del;
}


int ListPushHead (List* list, elem_t value)
{
    assert (list);

    node* new_elem = (node*) calloc(1, sizeof(node));

    if (list->size == 0) 
    {
        new_elem->next = 0;
        new_elem->prev = 0;
        list->head = new_elem;
        list->tail = new_elem;
    }
    else
    {
        new_elem->next = 0;
        new_elem->prev = list->head;
        list->head->next = new_elem;
    }

}


int ListPushTail (List* list, elem_t value)
{
    assert (list);

}


int ListInsert (List* list, elem_t value, int real_adr)
{
    
}


int ListFind (List* list, int id)
{

}


void _ListDump (List* list, const char* /*filename[]*/, const char func_name[], const int line)
{
    // printf ("\n--------------------------------------\n");
    
    // printf ("Call of function ListDump at %s, line %d:\n", func_name, line);

    // printf ("Head: %d, Tail: %d, Free: %d\n",
    //         list->head->value, list->tail->value, list->free->value);
    // printf ("Size: %d\nCapacity: %d\n", list->size, list->capacity);

    // // Output of the list element and their connections
    // printf ("\t prev \t value \t next\n");    
    // for (int elem_id = 0; elem_id < list->capacity; elem_id++)
    // {
    //     printf ("%d:\t %d \t %d \t %d\n",
    //             elem_id,
    //             list->data[elem_id].prev->value,
    //             list->data[elem_id].value,
    //             list->data[elem_id].next->value);
    // }


    // printf ("\n--------------------------------------\n");
}


void ListCtor (List* list, int capacity)
{
    list->size = 0;
    list->head = nullptr;
    list->tail = nullptr;
}


void ListDtor (List* list)
{
    list->size = -1;

    list->head = nullptr;
    list->tail = nullptr;
}

