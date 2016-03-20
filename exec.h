#ifndef EXEC_H_INCLUDED
#define EXEC_H_INCLUDED
#include "object.h"

typedef struct arg_list_t
{
    char *name;
    Tao_object *obj;
    struct arg_list_t *next;
};


#endif // EXEC_H_INCLUDED
