
#include "list.h"

int main ()
{
    List list = {};
    FILE* log_file = get_file ("../data/log.html", "rw");

    ListCtor (&list, LIST_INITIAL_CAPACITY);
    ListDump (&list);

    ListPushBack (&list, 10);
    ListDump (&list);
    ListPushBack (&list, 15);
    ListDump (&list);
    ListPushBack (&list, 20);
    ListDump (&list);
    ListInsertRight (&list, 99, 2);
    ListDump (&list);
    ListResize (&list, 12);
    ListDump (&list);
    ListInsertRight (&list, 300, 2);

    DumpList (&list);

    fclose (log_file);
    ListDtor (&list);
}





int ListDelete (List* self, int elem_id)
{
    node* elem_to_del = self->data + elem_id;

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
        node* prev_elem = self->data + elem_to_del->prev;
        node* next_elem = self->data + elem_to_del->next;

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


int ListPushFront (List* self, elem_t value)
{
    assert (self);

    if (self->size == self->capacity)
    {
        printf ("\nList is full, fucking fuck!\n\n");
        return 0;
    }

    // Finding free space, inserting new elem there.
    int new_elem_id = 0;
    node* new_elem = InitNewElem (self, &new_elem_id, value);

    // Size = 0 case
    if (HandleZeroSize (self, new_elem_id)) return new_elem_id;
    
    // Size != 0
    printf ("Inserting to head\n");
    new_elem->next = self->head;
    self->data[self->head].prev = new_elem_id;
    self->head = new_elem_id;

    self->size++;

    return new_elem_id;
}


int ListPushBack (List* self, elem_t value)
{
    assert (self);

    // Finding free space, inserting new elem there.
    int new_elem_id = 0;
    node* new_elem = InitNewElem (self, &new_elem_id, value);

    // Size = 0 case
    if (HandleZeroSize (self, new_elem_id)) return new_elem_id;
    
    // Size != 0
    printf ("Inserting to tail\n");
    new_elem->prev = self->tail;
    self->data[self->tail].next = new_elem_id;
    self->tail = new_elem_id;

    self->size++;

    return new_elem_id;
}


int ListInsertRight (List* self, elem_t value, int elem_id, int real_pos)
{
    if (real_pos == NOT_STATED) real_pos = GetRealPos (self, elem_id);

    if (real_pos == self->tail) return ListPushBack (self, value);
    
    // Finding free space
    int new_elem_id = self->free;
    self->free = self->data[self->free].next;

    // Changing connections of neighbours
    int right_neighbour_id = self->data[real_pos].next;
    self->data[right_neighbour_id].prev = new_elem_id;
    self->data[real_pos].next = new_elem_id;
    
    // Writing inserted elem data
    self->data[new_elem_id].value = value;
    self->data[new_elem_id].prev  = real_pos;
    self->data[new_elem_id].next  = right_neighbour_id;
    
    return new_elem_id;
}


int ListInsertLeft (List* self, elem_t value, int elem_id, int real_pos)
{
    if (real_pos == self->head) return ListPushFront (self, value);

    return ListInsertRight (self, value, elem_id - 1, real_pos);
}


node* InitNewElem (List* self, int* new_elem_id, elem_t value)
{
    // Finding free space
    *new_elem_id = self->free;
    node* new_elem = self->data + *new_elem_id;
    self->free = self->data[self->free].next;

    // Giving default values to node
    new_elem->value = value;
    new_elem->next  = 0;
    new_elem->prev  = 0;  

    return new_elem;
}


int HandleZeroSize (List* self, int new_elem_id)
{
    if (self->size == LIST_INITIAL_SIZE)
    {
        self->head = new_elem_id;
        self->tail = new_elem_id;
        self->size++;
        return 1;
    }
    return 0;
}


void ListResize (List* self, int new_capacity)
{
    if (new_capacity < self->capacity) printf ("Can't shrink the list, data may corrupt.\n");
    
    // Allocating space for the new list
    node* new_data = (node*) calloc (new_capacity, sizeof (node));
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


int GetRealPos (List* self, int id)
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
    ListVerificate (self);

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


void ListVerificate (List* self)
{
    if (!self)
    { 
        PutError (NULL_PTR);
        return;
    }

    if (self->size < 0 || self->size > self->capacity) PutError (INVALID_SIZE);
    if (self-> size == self->capacity) PutError (LIST_IS_FULL);

    for (int i = 1; i < self->size; i++)
    {
        int prev_indx = self->data[i].prev;
        int next_indx = self->data[i].next;

        if (self->data[prev_indx].next != i || self->data[next_indx].prev != i)
        {
            if (prev_indx != 0 && next_indx != 0) PutError (WRONG_CONNECTIONS);
        }
    }
}


void DumpList (List* self)
{
    #define _print(...) fprintf (dot_file, __VA_ARGS__)

    FILE* dot_file = get_file ("data/list.dot", "w+");
    
    // Writing header info
    const char header[] = R"(
    digraph g {
        dpi      = 100
        compound  =  true;
        newrank   =  true;
        rankdir   =  LR;

    )";
    _print (header);


    // Filling the value of each node
    for (int i = 0; i < self->capacity; i++) 
    {
        _print ( "node%d[shape=record, label=\" real address: %d | <p>prev: %d | value: %d | <n>next: %d\"] \n \n",
                i, i, self->data[i].prev, self->data[i].value, self->data[i].next);
    }

    // Making invisible connections
    _print ("node0");
    for (int i = 1; i < self->capacity; i++)
    {
        _print ("->node%d", i);
    }
    _print ("[style=invis, weight=1, minlen=\"1.5\"]\n");

    // Service data
    _print ("Free->node%d\n", self->free);
    _print ("Head->node%d\n", self->head);
    _print ("Tail->node%d\n", self->tail);

    // Making connections
    for (int i = 1; i < self->capacity; i++) 
    {
        _print ("node%d:p -> node%d \n", i, self->data[i].prev);
        _print ("node%d:n -> node%d \n", i, self->data[i].next);
    }

    _print ("}\n");

    #undef _print

    fclose (dot_file);
    system ("dot -Tpng data/list.dot -o data/result.png");
}

void ListCtor (List* self, int capacity)
{
    // Initializing the array of elems
    self->data = (node*) calloc (capacity, sizeof(node));
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


