#include "error_handler.h"


void PutError (int error_id, FILE* log_file)
{         
    switch (error_id)
    {
    case NULL_PTR:
        $print ("Function was given null-ptr, look at in-params");
        break;
    
    case INVALID_SIZE:
        $print ("Was given an invalid size");
        break;

    case WRONG_CONNECTIONS:
        $print ("List nodes are connected the wrong way");
        break;

    default:
        $print ("FUCK!!! Error code: %d", error_id);
        break;
    }
}
