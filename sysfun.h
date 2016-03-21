#ifndef SYSFUN_H_INCLUDED
#define SYSFUN_H_INCLUDED

#include "object.h"

#define is_empty(l) ((l)->next==NULL)

#define list_init_g(type) \
do{\
    type *not_use_tmp = malloc(sizeof(type));\
    not_use_tmp->next=NULL;\
    return not_use_tmp;\
}while(0);

typedef struct arg_list_t
{
    char *name;
    Tao_object *obj;
    struct arg_list_t *next;
}obj_list,symbol_list;

typedef Tao_object * (*sysfun)(obj_list *args);
typedef struct sysfun_list_t
{
    char *name;
    sysfun func;
    struct sysfun_list_t *next;
}sysfun_list;

symbol_list * symbol_list_init(void);
void symbol_list_add(symbol_list **l,char *name,Tao_object *obj);
sysfun_list * sysfun_list_init(void);
void sysfun_list_add(sysfun_list **l,char *name,sysfun func);
void add_sysfun(sysfun_list **l);
Tao_object * sys_print(obj_list *args);

#endif // SYSFUN_H_INCLUDED
