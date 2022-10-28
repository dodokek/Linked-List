#ifndef ASSERT_H
#define ASSERT_H

#include "list.h"


#define my_assert(X)                        \
        if (!(X))                           \
        {                                   \
            printf ("Assertion failed!");   \
            ListVerificate (list);          \
        }


enum ErrCodes
{
    NULL_PTR,
    INVALID_SIZE,
    WRONG_CONNECTIONS,
    LIST_IS_FULL = 0,
};


void PutError (int error_id, FILE* log_file);





#endif