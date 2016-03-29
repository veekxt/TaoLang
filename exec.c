#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exec.h"

void exec_error(char *s,AST *ast,int n)
{
    printf("line %d ,error: %s\n",ast->line,s);
    exit(n);
}

int is_same_type(Tao_value*a,Tao_value*b)
{
    if(a->type==b->type)
    {
        if(a->type!=C_USEROBJ)
        {
            return 1;
        }else
        {
            //判断obj类型
        }
    }else
    {
        return 0;
    }
    return 0;
}

int is_addble(Tao_value*a,Tao_value*b)
{
    if(is_same_type(a,b))
    {
        if(a->type==C_BOOL||a->type==C_NONE) return 0;
        if(a->type==C_USEROBJ){return 0;}//支持重载的话要补充这里
        //if(a->type==C_STR) return 0;
    }
    else
    {
        return 0;
    }
    return 1;
}

int is_redble(Tao_value*a,Tao_value*b)
{
    if(is_same_type(a,b))
    {
        if(a->type==C_BOOL||a->type==C_NONE) return 0;
        if(a->type==C_USEROBJ){return 0;}//支持重载的话要补充这里
        if(a->type==C_STR) return 0;
    }
    else
    {
        return 0;
    }
    return 1;
}

int is_mulble(Tao_value*a,Tao_value*b)
{
    if(is_same_type(a,b))
    {
        if(a->type==C_BOOL||a->type==C_NONE) return 0;
        if(a->type==C_USEROBJ){return 0;}//支持重载的话要补充这里
        if(a->type==C_STR) return 0;
    }
    else
    {
        return 0;
    }
    return 1;
}

int is_divble(Tao_value*a,Tao_value*b)
{
    if(is_same_type(a,b))
    {
        if(a->type==C_BOOL||a->type==C_NONE) return 0;
        if(a->type==C_USEROBJ){return 0;}//支持重载的话要补充这里
        if(a->type==C_STR) return 0;
    }
    else
    {
        return 0;
    }
    return 1;
}

int is_modble(Tao_value*a,Tao_value*b)
{
    if(is_same_type(a,b))
    {
        if(a->type==C_BOOL||a->type==C_NONE) return 0;
        if(a->type==C_USEROBJ){return 0;}//支持重载的话要补充这里
        if(a->type==C_STR) return 0;
    }
    else
    {
        return 0;
    }
    return 1;
}

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

Tao_value *add_two_value(Tao_value *a,Tao_value *b)
{
    switch(a->type)
    {
        case C_FLOAT:{
            Tao_value *tmp = malloc(sizeof(Tao_value));
            tmp->type=a->type;
            tmp->value.float_value.val=a->value.float_value.val+b->value.float_value.val;
            return tmp;
        }
        break;
        case C_INT:{
            Tao_value *tmp = malloc(sizeof(Tao_value));
            tmp->type=a->type;
            tmp->value.int_value.val=a->value.int_value.val+b->value.int_value.val;
            return tmp;
        }
        break;
    }
    return NULL;
}

Tao_value *red_two_value(Tao_value *a,Tao_value *b)
{
    switch(a->type)
    {
        case C_FLOAT:{
            Tao_value *tmp = malloc(sizeof(Tao_value));
            tmp->type=a->type;
            tmp->value.float_value.val=a->value.float_value.val - b->value.float_value.val;
            return tmp;
        }
        break;
        case C_INT:{
            Tao_value *tmp = malloc(sizeof(Tao_value));
            tmp->type=a->type;
            tmp->value.int_value.val=a->value.int_value.val-b->value.int_value.val;
            return tmp;
        }
        break;
    }
    return NULL;
}

Tao_value *mul_two_value(Tao_value *a,Tao_value *b)
{
    switch(a->type)
    {
        case C_FLOAT:{
            Tao_value *tmp = malloc(sizeof(Tao_value));
            tmp->type=a->type;
            tmp->value.float_value.val=a->value.float_value.val * b->value.float_value.val;
            return tmp;
        }
        break;
        case C_INT:{
            Tao_value *tmp = malloc(sizeof(Tao_value));
            tmp->type=a->type;
            tmp->value.int_value.val=a->value.int_value.val*b->value.int_value.val;
            return tmp;
        }
        break;
    }
    return NULL;
}

Tao_value *div_two_value(Tao_value *a,Tao_value *b)
{
    switch(a->type)
    {
        case C_FLOAT:{
            Tao_value *tmp = malloc(sizeof(Tao_value));
            tmp->type=a->type;
            tmp->value.float_value.val=a->value.float_value.val / b->value.float_value.val;
            return tmp;
        }
        break;
        case C_INT:{
            Tao_value *tmp = malloc(sizeof(Tao_value));
            tmp->type=a->type;
            tmp->value.int_value.val=a->value.int_value.val/b->value.int_value.val;
            return tmp;
        }
        break;
    }
    return NULL;
}

Tao_value *mod_two_value(Tao_value *a,Tao_value *b)
{
    switch(a->type)
    {
        case C_INT:{
            Tao_value *tmp = malloc(sizeof(Tao_value));
            tmp->type=a->type;
            tmp->value.int_value.val=a->value.int_value.val%b->value.int_value.val;
            return tmp;
        }
        break;
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
        break;
        case A_FLOAT:{
            Tao_value *obj = new_float(atof(ast->content));
            tar->return_value=obj;
            tar->result=R_NOR;
        }
        break;
        case A_STR:{
            Tao_value *obj = new_str(ast->content);
            tar->return_value=obj;
            tar->result=R_NOR;
        }
        break;
        #define make_left_right_ast \
        AST *ast_left = Taolist_get(AST*,0,ast->child);\
        AST *ast_right = Taolist_get(AST*,1,ast->child);\
        exec_result *left = cal_exp(ast_left,env);\
        exec_result *right = cal_exp(ast_right,env);\
        Tao_value *left_val = left->return_value;\
        Tao_value *right_val = right->return_value;\

        case A_ADD:{
            make_left_right_ast
            if(is_addble(left_val,right_val))
            {
                tar->return_value = add_two_value(left_val,right_val);
                tar->result=R_NOR;
            }else
            {
                exec_error("canot add ,type error",ast,1);
            }
        }
        break;
        case A_RED:{
            make_left_right_ast
            if(is_redble(left_val,right_val))
            {
                tar->return_value = red_two_value(left_val,right_val);
                tar->result=R_NOR;
            }else
            {
                exec_error("canot subtract ,type error",ast,1);
            }
        }
        break;
        case A_MUL:{
            make_left_right_ast
            if(is_mulble(left_val,right_val))
            {
                tar->return_value = mul_two_value(left_val,right_val);
                tar->result=R_NOR;
            }else
            {
                exec_error("canot multiply ,type error",ast,1);
            }
        }
        break;
        case A_DIV:{
            make_left_right_ast
            if(is_divble(left_val,right_val))
            {
                tar->return_value = div_two_value(left_val,right_val);
                tar->result=R_NOR;
            }else
            {
                exec_error("canot divide ,type error",ast,1);
            }
        }
        break;
        case A_MOD:{
            make_left_right_ast
            if(is_modble(left_val,right_val))
            {
                tar->return_value = mod_two_value(left_val,right_val);
                tar->result=R_NOR;
            }else
            {
                exec_error("canot mod ,type error",ast,1);
            }
        }
        break;
    }
    return tar;
}

exec_result *exec_funcall(AST *ast,exec_env *env)
{
    //执行函数调用
    sysfun func = sysfun_find(env->env_sysfun_list,ast->content);
    AST *args_ast = Taolist_get(AST*,0,ast->child);
    obj_list *args = symbol_list_init();

    for(int i=args_ast->child->len-1;i>=0;i--)
    {
        AST *arg_ast = Taolist_get(AST*,i,args_ast->child);
        exec_result *rs = cal_exp(arg_ast,env);
        symbol_list_add(&(args),arg_ast->content,rs->return_value);
    }
    func(args);
    return NULL;//todo return result
}

exec_result *exec_let(AST *ast,exec_env *env)
{
    AST *left = Taolist_get(AST*,0,ast->child);
    AST *right = Taolist_get(AST*,1,ast->child);
    symbol_list_add(&(env->env_symbol_table->head),left->content,cal_exp(right,env)->return_value);
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
