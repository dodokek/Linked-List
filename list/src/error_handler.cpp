#include "error_handler.h"


void PutError (int error_id)
{
    #define printf(...)                   \
        printf ("\n===========");       \
        printf (__VA_ARGS__);                     \
        printf ("============\n\n");              


    switch (error_id)
    {
    case NULL_PTR:
        printf ("Function was given null-ptr, look at in-params");
        break;
    
    case INVALID_SIZE:
        printf ("Was given an invalid size");
        break;

    case WRONG_CONNECTIONS:
        printf ("List nodes are connected the wrong way");
        break;

    default:
        printf ("FUCK!!! Error code: %d", error_id);
        break;
    }

    #undef printf
}
