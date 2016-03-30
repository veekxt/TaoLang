#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exec.h"

symbol_list * symbol_list_init(void)
{
    list_init_g(symbol_list);
}

void symbol_list_add(symbol_list **l,char *name,Tao_value *obj)
{
    symbol_list *head = malloc(sizeof(symbol_list));
    head->next=*l;
    head->name=name;
    head->obj=obj;
    *l=head;
}

sysfun_list * sysfun_list_init(void)
{
    list_init_g(sysfun_list);
}

sysfun sysfun_find(sysfun_list *l,char *s)
{
    while(l->next!=NULL)
    {
        if(strcmp(l->name,s)==0)
        {
            return l->func;
        }
        l=l->next;
    }
    return NULL;
}

void sysfun_list_add(sysfun_list **l,char *name,sysfun func)
{
    sysfun_list *head = malloc(sizeof(sysfun_list));
    head->next=*l;
    head->func=func;
    head->name=name;
    *l=head;
}

void add_sysfun(sysfun_list **l)
{
    sysfun_list_add(l,"print",sys_print);
    sysfun_list_add(l,"input",sys_input);
    sysfun_list_add(l,"int",sys_int);
    //...
    //...
}

Tao_value *sys_print(obj_list *args)
{
    Tao_value *tmp=malloc(sizeof(Tao_value));
    tmp->type=C_NONE;

    char *end="\n";
    while(!is_empty(args))
    {
        /*
        if(strcmp("end",args->name))
        {
            end = ((str_object *)args->obj->ce->ob_to_str(args->obj))->str;
        }
        */
        //printf(((str_object *)args->obj->ce->ob_to_str(args->obj))->str);
        switch(args->obj->type)
        {
        case C_INT:
            printf("%ld",args->obj->value.int_value.val);
        break;
        case C_FLOAT:
            printf("%f",args->obj->value.float_value.val);
        break;
        case C_BOOL:
            printf(args->obj->value.bool_value.val==0?"false":"true");
        break;
        case C_STR:
            printf("%s",args->obj->value.str_value.val);
        break;
        case C_NONE:
            printf("%s","none");
        break;
        case C_USEROBJ:
            ///todo
            //printf("%s","none");
        break;
        }
        args=args->next;
    }
    printf("%s",end);

    return tmp;//return none
}

Tao_value *sys_input(obj_list *args)
{
    sys_print(args);
    char *s=malloc(1001);
    scanf("%[^\n]",s);
    getchar();
    Tao_value *tmp = new_str(s);
    //free(s);
    tmp->type=C_STR;
    return tmp;
}

Tao_value *sys_int(obj_list *args)
{
    if(args->obj->type==C_STR)
    {
        return new_int(atol(args->obj->value.str_value.val));
    }
}
