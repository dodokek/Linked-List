#include "list.h"


int main()
{
    List list = {};
    FILE* log_file = get_file ("data/log.html", "w+");
    //$print ("<head><style>html {background-image: url('https://memepedia.ru/wp-content/uploads/2022/10/mudroe-tainstvennoe-derevo-mem-8.jpg');background-repeat: no-repeat;background-attachment: fixed;background-size: cover;}</style></head>\n");

    ListCtor (&list, LIST_INITIAL_CAPACITY);
    ListDump (&list, "Constructing list with capacity %d\n", LIST_INITIAL_CAPACITY);

    ListInsertTail (&list, 10);
    ListDump (&list, "Pushing back to tail value %d\n", 10);

    ListInsertTail (&list, 15);
    ListDump (&list, "Pushing back to tail value %d\n", 15);
    
    ListInsertTail (&list, 20);
    ListDump (&list, "Pushing back to tail value %d\n", 20);
    
    ListInsertRight (&list, 99, 2);
    ListDump (&list, "Inserting value %d to real address %d\n", 99, 2);
    
    ListResize (&list, 12);
    ListDump (&list, "Resing list, new capacity %d", 12);
    
    ListInsertTail (&list, 320);
    ListDump (&list, "Pushing back to tail value %d\n", 320);
    
    ListLinearize (&list);
    ListDump (&list, "Linearizing list", "");

    fclose (log_file);
    ListDtor (&list);
}