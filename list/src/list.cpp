
#include "list.h"

int main ()
{
    List list = {};

    ListCtor (&list, 10);

    list.data[2].value = 10;
    list.data[3].value = 15;
    list.data[4].value = 25;
    list.data[9].value = 100;

    ListDump (&list);

    ListDtor (&list);
}


void _ListDump (List* self, const char* /*filename[]*/, const char func_name[], const int line)
{
    printf ("\n--------------------------------------\n");
    
    printf ("Call of function ListDump at %s, line %d:\n", func_name, line);

    printf ("Head: %d, Tail: %d, Free: %d\n",
            self->head->value, self->tail->value, self->free->value);
    printf ("Size: %d\nCapacity: %d\n", self->size, self->capacity);

    // Output of the list element and their connections
    printf ("\t prev \t value \t next\n");    
    for (int elem_id = 0; elem_id < self->capacity; elem_id++)
    {
        printf ("%d:\t %d \t %d \t %d\n",
                elem_id,
                self->data[elem_id].prev->value,
                self->data[elem_id].value,
                self->data[elem_id].next->value);
    }


    printf ("\n--------------------------------------\n");
}


void ListCtor (List* self, int capacity)
{
    // Initializing the array of elems
    self->data = (ListElem*) calloc (capacity, sizeof(ListElem));
    assert (self->data);
    self->size = 0;
    self->capacity = capacity;

    // Handling the beginning and the end of list
    self->data[0].next = self->data;
    self->data[0].prev = self->data;
    self->data[capacity - 1].next = self->data;
    self->data[capacity - 1].prev = self->data + capacity - 2;

    // Making connections with free elems
    for (int elem_id = 1; elem_id < capacity - 1; elem_id++)
    {
        self->data[elem_id].next = self->data + elem_id + 1;
        self->data[elem_id].prev = self->data + elem_id - 1;
    }
    
    // Initialising the rest of stuff
    self->head = self->data + 1;
    self->tail = self->data + 1;
    self->free = self->data + 1;
}


void ListDtor (List* self)
{
    FREE(self->data);
    self->size = -1;

    self->head = nullptr;
    self->tail = nullptr;
    self->free = nullptr;
}

