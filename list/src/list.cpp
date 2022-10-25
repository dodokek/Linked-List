
#include "list.h"

int main ()
{
    List list = {};

    ListCtor (&list, LIST_INITIAL_CAPACITY);
    ListDump (&list);

    ListPushHeadTail (&list, 10, TAIL);
    ListDump (&list);
    ListPushHeadTail (&list, 15, TAIL);
    ListDump (&list);
    ListPushHeadTail (&list, 20, TAIL);
    ListDump (&list);
    ListInsert (&list, 99, 2);
    ListDump (&list);
    ListResize (&list, 12);
    ListDump (&list);


    ListDtor (&list);
}


void ListResize (List* self, int new_capacity)
{
    if (new_capacity < self->capacity) printf ("Can't shrink the list, data may corrupt.\n");
    
    // Allocating space for the new list
    ListElem* new_data = (ListElem*) calloc (new_capacity, sizeof (ListElem));
    assert (new_data);
    
    // Rewriting every element in linear order
    int cur_elem_id = self->head;

    for (int i = 1; i < self->size + 1; i++)
    {
        *(new_data + i) = self->data[cur_elem_id];
        cur_elem_id = self->data[cur_elem_id].next;
    }

    for (int elem_id = self->size + 1; elem_id < new_capacity; elem_id++)
    {
        new_data[elem_id].value = -1;
    }

    for (int i = 1; i < new_capacity - 1; i++)
    {
        new_data[i].prev = i - 1;
        new_data[i].next = i + 1;
    }

    new_data[new_capacity - 1].next = 0;
    new_data[new_capacity - 1].prev = new_capacity - 2;
    
    // Rewriting list info
    self->capacity = new_capacity;
    self->free = cur_elem_id;
    self->head = 1;
    self->tail = self->size;

    free (self->data);
    self->data = new_data;

    return;
}



int ListDelete (List* self, int elem_id)
{
    ListElem* elem_to_del = self->data + elem_id;

    if (self->size == LIST_INITIAL_SIZE + 1) // One elem in List case
    {
        self->tail = 0;
        self->head = 0;
    }
    else if (elem_id == self->tail) //Deleting tail
    {
        self->tail = self->data[self->tail].prev;
        self->data[self->tail].next = 0;
    }
    else if (elem_id == self->head) //Deleting head
    {
        self->head = self->data[self->head].next;
        self->data[self->head].prev = 0;
    }
    else
    {
        ListElem* prev_elem = self->data + elem_to_del->prev;
        ListElem* next_elem = self->data + elem_to_del->next;

        prev_elem->next = elem_to_del->next;
        next_elem->prev = elem_to_del->prev;
    }

    // Pushing freed elem to the beginning of free-data-list
    self->data[self->free].prev = elem_id;

    elem_to_del->value = -1;
    elem_to_del->next = self->free;
    elem_to_del->prev = 0;

    self->free = elem_id;
    self->size--;

    return elem_id;
}


int ListPushHeadTail (List* self, elem_t value, int push_mode)
{
    assert (self);

    if (self->size == self->capacity)
    {
        printf ("\nList is full, fucking fuck!\n\n");
        return 0;
    }

    // Finding free space
    int new_elem_id = self->free;
    ListElem* inserted_elem = self->data + new_elem_id;
    self->free = self->data[self->free].next;

    // Giving default values to ListElem
    inserted_elem->value = value;
    inserted_elem->next  = 0;
    inserted_elem->prev  = 0;

    if (self->size == LIST_INITIAL_SIZE)
    {
        self->head = new_elem_id;
        self->tail = new_elem_id;
        self->size++;
        return new_elem_id;
    }

    if (push_mode == TAIL)
    {
        printf ("Inserting to tail\n");
        inserted_elem->prev = self->tail;
        self->data[self->tail].next = new_elem_id;
        self->tail = new_elem_id;
    }

    if (push_mode == HEAD)
    {
        printf ("Inserting to head\n");
        inserted_elem->next = self->head;
        self->data[self->head].prev = new_elem_id;
        self->head = new_elem_id;
    }

    self->size++;

    return new_elem_id;
}


int ListInsert (List* self, elem_t value, int elem_id, int physical_indx)
{
    if (physical_indx == NOT_STATED) physical_indx = ListFind (self, elem_id);

    if (physical_indx == self->tail) return ListPushHeadTail (self, value, TAIL);
    if (physical_indx == self->head) return ListPushHeadTail (self, value, HEAD);
    
    // Finding free space
    int new_elem_id = self->free;
    self->free = self->data[self->free].next;

    // Changing connections of neighbours
    int right_neighbour_id = self->data[physical_indx].next;
    self->data[right_neighbour_id].prev = new_elem_id;
    self->data[physical_indx].next = new_elem_id;
    
    // Writing inserted elem data
    self->data[new_elem_id].value = value;
    self->data[new_elem_id].prev  = physical_indx;
    self->data[new_elem_id].next  = right_neighbour_id;
    
    return new_elem_id;
}


int ListFind (List* self, int id)
{
    printf ("Why don't you just save the address, fucker?\n");

    if (id > self->size - 1) return -1;

    int cur_elem_id = self->head;

    for (int i = 0; i < id - 1; i++)
    {
        cur_elem_id = self->data[cur_elem_id].next;
    }

    return cur_elem_id;
}


void _ListDump (List* self, const char* /*filename[]*/, const char func_name[], const int line)
{
    printf ("\n--------------------------------------\n");
    
    printf ("Call of function ListDump at %s, line %d:\n", func_name, line);

    printf ("Head: %d, Tail: %d, Free: %d\n",
            self->head, self->tail, self->free);
    printf ("Size: %d\nCapacity: %d\n", self->size, self->capacity);

    // Output of the list element and their connections
    printf ("\t prev \t value \t next\n");    
    for (int elem_id = 0; elem_id < self->capacity; elem_id++)
    {
        printf ("%d:\t %d \t %d \t %d\n",
                elem_id,
                self->data[elem_id].prev,
                self->data[elem_id].value,
                self->data[elem_id].next);
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
    self->data[0].next = 0;
    self->data[0].prev = 0;

    self->data[capacity - 1].next  = 0;
    self->data[capacity - 1].value = -1;
    self->data[capacity - 1].prev  = capacity - 2;

    // Making connections with free elems
    for (int elem_id = 1; elem_id < capacity - 1; elem_id++)
    {
        self->data[elem_id].value = -1;
        self->data[elem_id].next = elem_id + 1;
        self->data[elem_id].prev = elem_id - 1;
    }
    
    // Initialising the rest of stuff
    self->head = 0;
    self->tail = 0;
    self->free = 1;

    self->linear = true;
}


void ListDtor (List* self)
{
    FREE(self->data);
    self->size = -1;

    self->head = 0;
    self->tail = 0;
    self->free = 0;
    self->linear = false;
}

