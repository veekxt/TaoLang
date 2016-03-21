#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sysfun.h"

#define list_init_g(type) \
do{\
    type *not_use_tmp = malloc(sizeof(type));\
    not_use_tmp->next=NULL;\
    return tmp;\
}while(0);

symbol_list * symbol_list_init(void)
{
    list_init_g(symbol_list *);
}

void symbol_list_add(symbol_list **l,char *name,Tao_object *obj)
{
    symbol_list *head = malloc(sizeof(symbol_list));
    (*head)->next=*l;
    (*head)->name=name;
    (*head)->obj=obj;
    *l=head;
}

sysfun_list * sysfun_list_init(void)
{
    list_init_g(sysfun_list *);
}

void sysfun_list_add(sysfun_list **l,char *name,sysfun func)
{
    sysfun_list *head = malloc(sizeof(sysfun_list));
    (*head)->next=*l;
    (*head)->func=func;
    (*head)->name=name;
    *l=head;
}

void add_sysfun(sysfun_list **l)
{
    sysfun_list_add(l,"print",sys_print);
    //...
    //...
}

Tao_object * sys_print(arg_list *args)
{
    char *end="\n";
    while(!is_empty(args))
    {
        /*
        if(strcmp("end",args->name))
        {
            end = ((str_object *)args->obj->ce->ob_to_str(args->obj))->str;
        }
        */
        printf(((str_object *)args->obj->ce->ob_to_str(args->obj))->str);
        args=args->next;
    }
    printf("%s",end);
    return none_new();//return none
}
