#ifndef LOG_H
#define LOG_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define $print(...) fprintf (LOG_FILE, __VA_ARGS__)

FILE* LOG_FILE = nullptr;




#endif