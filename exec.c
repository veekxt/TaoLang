#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exec.h"

symbol_table * symbol_table_init(void)
{
    list_init_g(symbol_table);
}

void symbol_table_add(symbol_table **l,symbol_list *s_head)
{
    symbol_table *head = malloc(sizeof(symbol_table));
    head->next=*l;
    head->head=s_head;
    *l=head;
}

Tao_value * symbol_list_find(symbol_list *l,char *s)
{
    while(l->next!=NULL)
    {
        if(strcmp(l->name,s)==0)
        {
            return l->obj;
        }
        l=l->next;
    }
    return NULL;
}

Tao_value * symbol_table_find(symbol_table *l,char *s)
{
    while(l->next!=NULL)
    {
        Tao_value *tmp=symbol_list_find(l->head,s);
        if(tmp==NULL)
        {
            l=l->next;
            continue;
        }else
        {
            return tmp;
            break;
        }
    }
    return NULL;
}

exec_env *make_init_env(void)
{
    exec_env *env = malloc(sizeof(exec_env));
    env->env_symbol_table = symbol_table_init();
    env->env_sysfun_list = sysfun_list_init();
    symbol_table_add(&env->env_symbol_table,symbol_list_init());
    add_sysfun(&env->env_sysfun_list);
    return env;
}
exec_result *cal_exp(AST *ast,exec_env *env)
{
    exec_result *tar = malloc(sizeof(exec_result));
    switch(ast->type)
    {
        case A_IDEN:{
            Tao_value *obj = symbol_table_find(env->env_symbol_table,ast->content);
            tar->return_value=obj;
            tar->result=R_NOR;
        }
        break;
        case A_INT:{
            Tao_value *obj = new_int(atoi(ast->content));
            tar->return_value=obj;
            tar->result=R_NOR;
        }
    }
    return tar;
}

exec_result *exec_funcall(AST *ast,exec_env *env)
{
    //执行函数调用
    sysfun func = sysfun_find(env->env_sysfun_list,ast->content);
    AST *args_ast = Taolist_get(AST*,0,ast->child);
    obj_list *args = symbol_list_init();

    for(int i=0;i<args_ast->child->len;i++)
    {
        AST *arg_ast = Taolist_get(AST*,i,args_ast->child);
        exec_result *rs = cal_exp(arg_ast,env);
        symbol_list_add(&(args),arg_ast->content,rs->return_value);
    }
    func(args);
    puts("a fun call");
    return NULL;//todo return result
}

exec_result *exec_let(AST *ast,exec_env *env)
{
    AST *left = Taolist_get(AST*,0,ast->child);
    AST *right = Taolist_get(AST*,1,ast->child);
    //简单测试，右边仅仅是一个数字
    symbol_list_add(&(env->env_symbol_table->head),left->content,cal_exp(right,env)->return_value);
    printf("add a int\n");
    return NULL;//todo return result
}

exec_result *exec_stmt(AST *ast,exec_env *env)
{
    for(int i = 0;i<ast->child->len;i++)
    {
        AST *a_child = Taolist_get(AST *,i,ast->child);
        switch(a_child->type)
        {
            case A_LET:
            {
                exec_let(a_child,env);
            }
            break;
            case A_FUNCALL:
            {
                exec_funcall(a_child,env);
            }
            break;
            default:;
        }
    }
    return NULL;//todo return result
}
