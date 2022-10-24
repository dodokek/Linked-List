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

#define FREE(x) free(x), x = nullptr

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


struct ListElem
{
    elem_t value;
    int prev;
    int next;
};


struct List
{
    ListElem* data;
    int size;
    int capacity;

    int head;
    int tail;
    int free;
    
};

int ListPushHeadTail (List* self, elem_t value, int push_mode);

int ListInsert (List* self, elem_t value, int elem_id, int physical_indx = NOT_STATED);

int ListFind (List* self, int id);

int ListDelete (List* self, int elem_id);

void ListCtor (List* self, int capacity);

void ListDtor (List* self);

void _ListDump (List* self, const char* /*filename[]*/, const char func_name[], const int line);

#endif