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


//----------------------------------------------

const int LIST_INITIAL_SIZE = 10;

//----------------------------------------------


struct ListElem
{
    elem_t value;
    ListElem* prev;
    ListElem* next;
};


struct List
{
    ListElem* data;
    int size;
    int capacity;

    ListElem* head;
    ListElem* tail;
    ListElem* free;
    
};


void ListCtor (List* self, int capacity);

void ListDtor (List* self);

void _ListDump (List* self, const char* /*filename[]*/, const char func_name[], const int line);

#endif