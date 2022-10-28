#ifndef LOG_H
#define LOG_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define $print(...) fprintf (log_file, __VA_ARGS__)

void LogList (FILE* log_file);




#endif