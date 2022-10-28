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
#include "error_handler.h"
#include "fileUtils.h"

//----------------------------------------------
// Fucking global variable!

int DUMP_NUMBER = 0;

//----------------------------------------------

#define FREE(x) free(x), x = nullptr

#define ListDump(X) _ListDump(X, __FILE__, __PRETTY_FUNCTION__, __LINE__, log_file)

#define $print(...) fprintf (log_file, __VA_ARGS__)

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

enum PushType
{
    TAIL,
    HEAD
};

enum VerificationMode
{
    LOUD,
    QUIET
};

//----------------------------------------------

const int MAX_IMG_SRC_LEN = 150;
const int POISON_NUM = 0xBEBDA;

//----------------------------------------------


struct node
{
    elem_t value;
    int prev;
    int next;
};


struct List
{
    node* data;
    int size;
    int capacity;

    int head;
    int tail;
    int free;
    
    bool linear;
};

int ListPushBack (List* self, elem_t value);

int ListPushFront (List* self, elem_t value);

node* InitNewElem (List* self, int* new_elem_id, elem_t value);

void ListVerificate (List* self, FILE* log_file);

int HandleZeroSize (List* self, int new_elem_id);

int ListInsertRight (List* self, elem_t value, int elem_id);

int ListInsertLeft (List* self, elem_t value, int elem_id);

void ListResize (List* self, int new_capacity);

int GetRealPos (List* self, int id);

int ListDelete (List* self, int elem_id);

void ListCtor (List* self, int capacity);

void ListDtor (List* self);

void _ListDump (List* self, const char* /*filename[]*/, const char func_name[], const int line, FILE* log_file);

void DrawList (List* self, FILE* log_file);
#endif