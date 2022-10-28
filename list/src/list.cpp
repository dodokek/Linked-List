
#include "list.h"

int main ()
{
    List list = {};
    FILE* log_file = get_file ("data/log.html", "w+");
    //$print ("<head><style>html {background-image: url('https://memepedia.ru/wp-content/uploads/2022/10/mudroe-tainstvennoe-derevo-mem-8.jpg');background-repeat: no-repeat;background-attachment: fixed;background-size: cover;}</style></head>\n");

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
    ListInsertRight (&list, 300, 3);
    ListDump (&list);
    ListLinearize (&list);
    ListDump (&list);

    

    fclose (log_file);
    ListDtor (&list);
}


int ListDelete (List* list, int elem_id)
{
    node* elem_to_del = list->data + elem_id;

    if (list->size == LIST_INITIAL_SIZE + 1) // One elem in List case
    {
        list->tail = 0;
        list->head = 0;
    }
    else if (elem_id == list->tail) //Deleting tail
    {
        list->tail = list->data[list->tail].prev;
        list->data[list->tail].next = 0;
    }
    else if (elem_id == list->head) //Deleting head
    {
        list->head = list->data[list->head].next;
        list->data[list->head].prev = 0;
        list->linear = false;
    }
    else
    {
        node* prev_elem = list->data + elem_to_del->prev;
        node* next_elem = list->data + elem_to_del->next;

        prev_elem->next = elem_to_del->next;
        next_elem->prev = elem_to_del->prev;
        list->linear = false;
    }

    // Pushing freed elem to the beginning of free-data-list
    list->data[list->free].prev = elem_id;

    elem_to_del->value = POISON_NUM;
    elem_to_del->next = list->free;
    elem_to_del->prev = 0;

    list->free = elem_id;
    list->size--;

    return elem_id;
}


int ListPushFront (List* list, elem_t value)
{
    assert (list);

    if (list->size == list->capacity)
    {
        printf ("\nList is full, fucking fuck!\n\n");
        return 0;
    }

    // Finding free space, inserting new elem there.
    int new_elem_id = 0;
    node* new_elem = InitNewElem (list, &new_elem_id, value);

    // Size = 0 case
    if (HandleZeroSize (list, new_elem_id)) return new_elem_id;
    
    // Size != 0
    printf ("Inserting to head\n");
    new_elem->next = list->head;
    list->data[list->head].prev = new_elem_id;
    list->head = new_elem_id;

    list->size++;

    return new_elem_id;
}


int ListPushBack (List* list, elem_t value)
{
    assert (list);

    // Finding free space, inserting new elem there.
    int new_elem_id = 0;
    node* new_elem = InitNewElem (list, &new_elem_id, value);

    // Size = 0 case
    if (HandleZeroSize (list, new_elem_id)) return new_elem_id;
    
    // Size != 0
    printf ("Inserting to tail\n");
    new_elem->prev = list->tail;
    list->data[list->tail].next = new_elem_id;
    list->tail = new_elem_id;

    list->size++;
    list->linear = false;

    return new_elem_id;
}


int ListInsertRight (List* list, elem_t value, int real_pos)
{
    if (real_pos == list->tail) return ListPushBack (list, value);
    
    // Finding free space
    int new_elem_id = list->free;
    list->free = list->data[list->free].next;

    // Changing connections of neighbours
    int right_neighbour_id = list->data[real_pos].next;
    list->data[right_neighbour_id].prev = new_elem_id;
    list->data[real_pos].next = new_elem_id;
    
    // Writing inserted elem data
    list->data[new_elem_id].value = value;
    list->data[new_elem_id].prev  = real_pos;
    list->data[new_elem_id].next  = right_neighbour_id;

    list->linear = false;
    list->size++;
    
    return new_elem_id;
}


int ListInsertLeft (List* list, elem_t value, int real_pos)
{
    if (real_pos == list->head) return ListPushFront (list, value);

    // Finding free space
    int new_elem_id = list->free;
    list->free = list->data[list->free].next;

    // Changing connections of neighbours
    int left_neighbour_id = list->data[real_pos].prev;
    list->data[left_neighbour_id].next = new_elem_id;
    list->data[real_pos].prev = new_elem_id;
    
    // Writing inserted elem data
    list->data[new_elem_id].value = value;
    list->data[new_elem_id].next  = real_pos;
    list->data[new_elem_id].prev  = left_neighbour_id;

    list->linear = false;
    list->size++;
    
    return new_elem_id;
}





node* InitNewElem (List* list, int* new_elem_id, elem_t value)
{
    // Finding free space
    *new_elem_id = list->free;
    node* new_elem = list->data + *new_elem_id;
    list->free = list->data[list->free].next;

    // Giving default values to node
    new_elem->value = value;
    new_elem->next  = 0;
    new_elem->prev  = 0;  

    return new_elem;
}


int HandleZeroSize (List* list, int new_elem_id)
{
    if (list->size == LIST_INITIAL_SIZE)
    {
        list->head = new_elem_id;
        list->tail = new_elem_id;
        list->size++;
        return 1;
    }

    return 0;
}


void ListResize (List* list, int new_capacity)
{
    if (new_capacity <= list->capacity) printf ("Can't shrink the list, data may corrupt.\n");
    
    // Allocating space for the new list
    node* new_data = (node*) calloc (new_capacity, sizeof (node));
    assert (new_data);
    
    // Rewriting every element in linear order
    int cur_elem_id = list->head;

    for (int i = 1; i < list->size + 1; i++)
    {
        *(new_data + i) = list->data[cur_elem_id];
        cur_elem_id = list->data[cur_elem_id].next;
    }

    for (int elem_id = list->size + 1; elem_id < new_capacity; elem_id++)
    {
        new_data[elem_id].value = POISON_NUM;
    }

    for (int i = 1; i < new_capacity - 1; i++)
    {
        new_data[i].prev = i - 1;
        new_data[i].next = i + 1;
    }

    new_data[list->size].next = 0;
    new_data[new_capacity - 1].next = new_capacity - 1;
    new_data[new_capacity - 1].prev = new_capacity - 2;
    
    // Rewriting list info
    list->linear = true;
    list->capacity = new_capacity;
    list->free = list->size + 1;
    list->head = 1;
    list->tail = list->size;

    free (list->data);
    list->data = new_data;

    return;
}


// He he
void ListLinearize (List* list)
{
    // Rewriting every element in linear order
    int cur_elem_id = list->head;

    for (int i = 1; i < list->size + 1; i++)
    {
        *(list->data + i) = list->data[cur_elem_id];
        cur_elem_id = list->data[cur_elem_id].next;
    }

    for (int elem_id = list->size + 1; elem_id < list->capacity; elem_id++)
    {
        list->data[elem_id].value = POISON_NUM;
    }

    for (int i = 1; i < list->capacity - 1; i++)
    {
        list->data[i].prev = i - 1;
        list->data[i].next = i + 1;
    }

    list->data[list->capacity - 1].next = list->capacity - 1;
    list->data[list->capacity - 1].prev = list->capacity - 2;

    // Rewriting list info
    list->linear = true;
    list->free = list->size + 1;
    list->head = 1;
    list->tail = list->size;
}


int GetRealPos (List* list, int id)
{
    printf ("Why don't you just save the address, fucker?\n");

    if (id > list->size - 1) return -1;
    if (list->linear) return id;

    int cur_elem_id = list->head;

    for (int i = 0; i < id - 1; i++)
    {
        cur_elem_id = list->data[cur_elem_id].next;
    }
    return cur_elem_id;
}


void ListVerificate (List* list, FILE* log_file)
{
    if (!list)
    { 
        PutError (NULL_PTR, log_file);
        return;
    }

    if (list->size < 0 || list->size > list->capacity) PutError (INVALID_SIZE, log_file);
    if (list-> size == list->capacity) PutError (LIST_IS_FULL, log_file);

    for (int i = 1; i < list->size; i++)
    {
        int prev_indx = list->data[i].prev;
        int next_indx = list->data[i].next;

        if (list->data[prev_indx].next != i || list->data[next_indx].prev != i)
        {
            if (prev_indx != 0 && next_indx != 0) PutError (WRONG_CONNECTIONS, log_file);
        }
    }
}


void _ListDump (List* list, const char* /*filename[]*/, const char func_name[], const int line, FILE* log_file)
{
    ListVerificate (list, log_file);

    

    $print ("<hr>\n<pre>\n");

    $print ("\n--------------------------------------\n");
    
    $print ("Call of function ListDump at %s, line %d:\n", func_name, line);

    $print ("Head: %d, Tail: %d, Free: %d\n",
            list->head, list->tail, list->free);
    $print ("Size: %d\nCapacity: %d\n", list->size, list->capacity);

    // Output of the list element and their connections
    $print ("\t prev \t value \t next\n");    
    for (int elem_id = 0; elem_id < list->capacity; elem_id++)
    {
        $print ("%d:\t %d \t %d \t %d\n",
                elem_id,
                list->data[elem_id].prev,
                list->data[elem_id].value,
                list->data[elem_id].next);
    }

    $print ("\n--------------------------------------\n</hr>");

    DrawList (list, log_file);
}


void DrawList (List* list, FILE* log_file)
{
    #define _print(...) fprintf (dot_file, __VA_ARGS__)

    FILE* dot_file = get_file ("data/list.dot", "w+");
    
    // Writing header info
    const char header[] = R"(
    digraph g {
        dpi      = 200;
        fontname = "Comic Sans MS";
        fontsize = 20;
        rankdir   =  TB;
        edge [color = darkgrey, arrowhead = onormal, arrowsize = 1, penwidth = 1.2]
        graph[fillcolor = lightgreen, ranksep = 1.3, nodesep = 0.5,
        style = "rounded, filled",color = green, penwidth = 2]

    )";
    _print (header);
    
    //Info node
    _print ("InfoNode[shape=record, color=\"red\", width=0.2, style=\"filled\", fillcolor=\"lightblue\", label=\" {Linear: %d | Size: %d | Capacity: %d}\"] \n \n",
                    list->linear, list->size, list->capacity);

    // Filling the value of each node
    for (int i = 0; i < list->capacity; i++) 
    {
        if (i == 0)
        {
            _print ("node%d[shape=record, width=0.2, style=\"filled\", fillcolor=\"grey\", label=\" {id: %d | <p>prev: %s | value: %s | <n>next: %s}\"] \n \n",
                    i, i, "nill", "nill", "nill");
        }
        else if (list->data[i].value == POISON_NUM)
        {
            _print ("node%d[shape=record, width=0.2, style=\"filled\", fillcolor=\"blue\", label=\" {id: %d | <p>prev: %s | value: %s | <n>next: %s}\"] \n \n",
                    i, i, "FREE", "POISON", "FREE");
        }
        else
        {
            if (i == list->head)
            {
                _print ("node%d[shape=record, width=0.2, style=\"filled\", fillcolor=\"darkgoldenrod3\", label=\" {id: %d | <p>prev: %d | value: %d | <n>next: %d}\"] \n \n",
                        i, i, list->data[i].prev, list->data[i].value, list->data[i].next);
            }
            else if (i == list->tail)
            {
                _print ("node%d[shape=record, width=0.2, style=\"filled\", fillcolor=\"darkgoldenrod1\", label=\" {id: %d | <p>prev: %d | value: %d | <n>next: %d}\"] \n \n",
                        i, i, list->data[i].prev, list->data[i].value, list->data[i].next);
            }
            else
            {
                _print ("node%d[shape=record, width=0.2, style=\"filled\", fillcolor=\"green\", label=\" {id: %d | <p>prev: %d | value: %d | <n>next: %d}\"] \n \n",
                        i, i, list->data[i].prev, list->data[i].value, list->data[i].next);
            }
        }
        
    }

    // Setting the same rank
    _print ("{ rank = same; ");
    for (int i = 0; i < list->capacity; i++) 
    {
        _print ("node%d; ", i);
    }
    _print ("}\n");

    // Service data
    _print ("Free->node%d\n", list->free);
    _print ("Head->node%d\n", list->head);
    _print ("Tail->node%d\n", list->tail);

    // Making invisible connections
    _print ("edge[style=invis, constraint = true]\n");
    _print ("node0");
    for (int i = 1; i < list->capacity; i++)
    {
        _print ("->node%d", i);
    }

    // Making real connections
    _print ("\n edge[style=solid, constraint = false]\n");

    for (int i = 1; i < list->capacity; i++) 
    {
        if (list->data[i].value == POISON_NUM && list->data[i].next != i)
            _print ("node%d:n -> node%d \n", i, list->data[i].next);

        else
        {
            if (list->data[i].prev != 0)
                _print ("node%d:p -> node%d \n", i, list->data[i].prev);

            if (list->data[i].next != 0)
                _print ("node%d:n -> node%d \n", i, list->data[i].next);
        }
    }

    _print ("}\n");

    #undef _print
    fclose (dot_file);

    // Executing dotfile and printing an image
    char img_tag[MAX_IMG_SRC_LEN] = "";
    char dot_cmd[MAX_IMG_SRC_LEN]  = "";
    sprintf (img_tag, "<img width=\"1400px\" src=\"../data/graph_%d.png\"> \n</pre>", DUMP_NUMBER);
    sprintf (dot_cmd, "dot -Tpng data/list.dot -o data/graph_%d.png", DUMP_NUMBER);

    system (dot_cmd);
    $print (img_tag);

    DUMP_NUMBER++;
}

void ListCtor (List* list, int capacity)
{
    // Initializing the array of elems
    list->data = (node*) calloc (capacity, sizeof(node));
    assert (list->data);
    list->size = LIST_INITIAL_SIZE; // 0 at the end of list
    list->capacity = capacity;

    // Handling the beginning and the end of list
    list->data[0].next = 0;
    list->data[0].prev = 0;

    list->data[capacity - 1].next  = capacity - 1;
    list->data[capacity - 1].value = POISON_NUM;
    list->data[capacity - 1].prev  = capacity - 2;

    // Making connections with free elems
    for (int elem_id = 1; elem_id < capacity - 1; elem_id++)
    {
        list->data[elem_id].value = POISON_NUM;
        list->data[elem_id].next = elem_id + 1;
        list->data[elem_id].prev = elem_id - 1;
    }
    
    // Initialising the rest of stuff
    list->head = 0;
    list->tail = 0;
    list->free = 1;

    list->linear = true;
}


void ListDtor (List* list)
{
    FREE(list->data);
    list->size = -1;

    list->head = 0;
    list->tail = 0;
    list->free = 0;
    list->linear = false;
}


