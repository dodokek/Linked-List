#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <assert.h>
#include <stdint.h>

#include "config.h"


//----------------------------------------------


#define ListDump(X) _ListDump(X, __FILE__, __PRETTY_FUNCTION__, __LINE__)

//----------------------------------------------

enum ListFlags
{
    NOT_STATED = -1,
};

enum Sizes
{
    LIST_INITIAL_CAPACITY = 10,
    LIST_INITIAL_SIZE = 1,
};

enum ERR_CODES
{
    LIST_IS_FULL = 0,
};

enum PUSH_TYPE
{
    TAIL,
    HEAD
};
//----------------------------------------------


struct node
{
    elem_t value;
    node* prev;
    node* next;
};


struct List
{
    int size;

    node* head;
    node* tail;
};

node* ListPushHeadTail (List* list, elem_t value, int push_mode);

node* ListInsert (List* list, elem_t value, int pos, int physical_indx);

node* ListDelete (List* list, int pos);

void ListCtor (List* list, int capacity);

void ListDtor (List* list);

int ListFind (List* list, int id);

void _ListDump (List* list, const char* /*filename[]*/, const char func_name[], const int line);

#endif