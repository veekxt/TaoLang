#ifndef SYSFUN_H_INCLUDED
#define SYSFUN_H_INCLUDED

#include "object.h"

#define is_empty(l) ((l)->next==NULL)

typedef struct arg_list_t
{
    char *name;
    Tao_object *obj;
    struct arg_list_t *next;
}obj_list,symbol_list;

typedef Tao_object * (*sysfun)(arg_list *args);
typedef struct sysfun_list_t
{
    char *name;
    sysfun func;
    struct sysfun_list_t *next;
}sysfun_list;


#endif // SYSFUN_H_INCLUDED
