#ifndef ASSERT_H
#define ASSERT_H

#include "list.h"


#define my_assert(X)                        \
        if (!(X))                           \
        {                                   \
            printf ("Assertion failed!");   \
            ListVerificate (self);          \
        }


enum ErrCodes
{
    NULL_PTR,
    INVALID_SIZE,
    WRONG_CONNECTIONS,
    LIST_IS_FULL = 0,
};


void PutError (int error_id);





#endif