
#include "list.h"

int main ()
{
    List list = {};

    ListCtor (&list, LIST_INITIAL_CAPACITY);

    ListPushHeadTail (&list, 10, TAIL);
    ListDump (&list);
    ListPushHeadTail (&list, 15, TAIL);
    ListDump (&list);
    ListPushHeadTail (&list, 20, TAIL);
    ListDump (&list);
    
    ListDelete (&list, 3);
    ListDump (&list);

    // list.data[2].value = 10;
    // list.data[3].value = 15;
    // list.data[4].value = 25;
    // list.data[9].value = 100;


    ListDtor (&list);
}


ListElem* ListDelete (List* self, int pos)
{
    ListElem* elem_to_del = self->data + pos;

    if (self->size == LIST_INITIAL_SIZE + 1) // One elem in List case
    {
        self->tail = self->data;
        self->head = self->data;
    }
    else if (elem_to_del == self->tail) //Deleting tail
    {
        self->tail = self->tail->prev;
        self->tail->next = self->data;
    }
    else if (elem_to_del == self->head) //Deleting head
    {
        self->head = self->head->next;
        self->head->prev = self->data;
    }
    else
    {
        ListElem* prev_elem = elem_to_del->prev;
        ListElem* next_elem = elem_to_del->next;

        prev_elem->next = next_elem;
        next_elem->prev = prev_elem;
    }

    // Pushing freed elem to the beginning of free-data-list
    self->free->prev = elem_to_del;
    elem_to_del->value = -1;
    elem_to_del->next = self->free;
    elem_to_del->prev = self->data;
    self->free = elem_to_del;

    return elem_to_del;
}


ListElem* ListPushHeadTail (List* self, elem_t value, int push_mode)
{
    assert (self);

    if (self->size == self->capacity)
    {
        printf ("\nList is full, fucking fuck!\n\n");
        return 0;
    }
    // Finding free space
    ListElem* inserted_elem = self->free;
    self->free = self->free->next;

    // Giving default values to ListElem
    inserted_elem->value = value;
    inserted_elem->next  = self->data;
    inserted_elem->prev  = self->data;

    if (self->size == LIST_INITIAL_SIZE)
    {
        self->head = inserted_elem;
        self->tail = inserted_elem;
        self->size++;
        return inserted_elem;
    }

    if (push_mode == TAIL)
    {
        printf ("Inserting to tail\n");
        inserted_elem->prev = self->tail;
        self->tail->next = inserted_elem;
        self->tail = inserted_elem;
    }

    if (push_mode == HEAD)
    {
        printf ("Inserting to head\n");
        inserted_elem->next = self->head;
        self->head->prev = inserted_elem;
        self->head = inserted_elem;
    }

    self->size++;

    return inserted_elem;
}


ListElem* ListInsert (List* self, elem_t value, int pos, int physical_indx = NOT_STATED)
{

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
    self->size = LIST_INITIAL_SIZE; // 0 at the end of list
    self->capacity = capacity;

    // Handling the beginning and the end of list
    self->data[0].next = self->data;
    self->data[0].prev = self->data;

    self->data[capacity - 1].next  = self->data;
    self->data[capacity - 1].value = -1;
    self->data[capacity - 1].prev  = self->data + capacity - 2;

    // Making connections with free elems
    for (int elem_id = 1; elem_id < capacity - 1; elem_id++)
    {
        self->data[elem_id].value = -1;
        self->data[elem_id].next = self->data + elem_id + 1;
        self->data[elem_id].prev = self->data + elem_id - 1;
    }
    
    // Initialising the rest of stuff
    self->head = self->data;
    self->tail = self->data;
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

