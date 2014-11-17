#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <strproc.h>
#include <tatgdata.h>

#define liststart "[list "
#define listend "]"
#define listsep " "
#define arraystart "ARRAY "
#define structstart "STRUCT "

int ParseASN1(char* s, char* d);