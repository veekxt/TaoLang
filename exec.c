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
        //if(a->type==C_STR) return 0;//允许字符串相加
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_redble(Tao_value*a,Tao_value*b)
{
    if(is_same_type(a,b))
    {
        if(a->type==C_BOOL||a->type==C_NONE) return 0;
        if(a->type==C_USEROBJ){return 0;}//支持重载的话要补充这里
        if(a->type==C_STR) return 0;
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_mulble(Tao_value*a,Tao_value*b)
{
    if(is_same_type(a,b))
    {
        if(a->type==C_BOOL||a->type==C_NONE) return 0;
        if(a->type==C_USEROBJ){return 0;}//支持重载的话要补充这里
        if(a->type==C_STR) return 0;
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_divble(Tao_value*a,Tao_value*b)
{
    if(is_same_type(a,b))
    {
        if(a->type==C_BOOL||a->type==C_NONE||a->type==C_STR) return 0;
        if(a->type==C_USEROBJ){return 0;}//支持重载的话要补充这里
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_modble(Tao_value*a,Tao_value*b)
{
    if(is_same_type(a,b))
    {
        if(a->type==C_BOOL||a->type==C_NONE) return 0;
        if(a->type==C_USEROBJ){return 0;}//支持重载的话要补充这里
        if(a->type==C_STR) return 0;
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_notble(Tao_value*a)
{
    if(a->type==C_BOOL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_minusble(Tao_value*a)
{
    if(a->type==C_INT || a->type==C_FLOAT)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_bigerble(Tao_value*a,Tao_value*b)
{
    if(is_same_type(a,b))
    {
        if(a->type==C_BOOL||a->type==C_NONE||a->type==C_STR) return 0;
        if(a->type==C_USEROBJ){return 0;}//支持重载的话要补充这里
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_bigereqble(Tao_value*a,Tao_value*b)
{
    if(is_same_type(a,b))
    {
        if(a->type==C_BOOL||a->type==C_NONE||a->type==C_STR) return 0;
        if(a->type==C_USEROBJ){return 0;}//支持重载的话要补充这里
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_smallereqble(Tao_value*a,Tao_value*b)
{
    if(is_same_type(a,b))
    {
        if(a->type==C_BOOL||a->type==C_NONE||a->type==C_STR) return 0;
        if(a->type==C_USEROBJ){return 0;}//支持重载的话要补充这里
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_smallerble(Tao_value*a,Tao_value*b)
{
    if(is_same_type(a,b))
    {
        if(a->type==C_BOOL||a->type==C_NONE||a->type==C_STR) return 0;
        if(a->type==C_USEROBJ){return 0;}//支持重载的话要补充这里
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_equalble(Tao_value*a,Tao_value*b)
{
    if(is_same_type(a,b))
    {
        if(a->type==C_BOOL||a->type==C_NONE||a->type==C_STR) return 0;
        if(a->type==C_USEROBJ){return 0;}//支持重载的话要补充这里
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_noteqble(Tao_value*a,Tao_value*b)
{
    if(is_same_type(a,b))
    {
        if(a->type==C_NONE) return 0;
        if(a->type==C_USEROBJ){return 0;}//支持重载的话要补充这里
        return 1;
    }
    else
    {
        return 0;
    }
}

int is_andble(Tao_value*a,Tao_value*b)
{
    if(is_same_type(a,b))
    {
        if(a->type==C_BOOL)return 1;
        return 0;
    }
    else
    {
        return 0;
    }
}

int is_orble(Tao_value*a,Tao_value*b)
{
    if(is_same_type(a,b))
    {
        if(a->type==C_BOOL) return 1;
        return 0;
    }
    else
    {
        return 0;
    }
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

void symbol_table_del_now(symbol_table **l)
{
    if(l==NULL) return;
    symbol_table *tmp =*l;
    if(*l !=NULL)(*l) = (*l)->next;
    symbol_list_kill(tmp->head);
    free(tmp);
}

symbol_list * symbol_list_find(symbol_list *l,char *s)
{
    while(l->next!=NULL)
    {
        if(strcmp(l->name,s)==0)
        {
            return l;
        }
        l=l->next;
    }
    return NULL;
}

symbol_list * symbol_table_find(symbol_table *l,char *s)
{
    while(l->next!=NULL)
    {
        symbol_list *tmp=symbol_list_find(l->head,s);
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

Tao_value *not_value(Tao_value *a)
{
    switch(a->type)
    {
        case C_BOOL:{
            Tao_value *tmp = malloc(sizeof(Tao_value));
            tmp->type=a->type;
            tmp->value.bool_value.val=a->value.bool_value.val==0?1:0;
            return tmp;
        }
        break;
    }
    return NULL;
}

Tao_value *minus_value(Tao_value *a)
{
    switch(a->type)
    {
        case C_FLOAT:{
            Tao_value *tmp = malloc(sizeof(Tao_value));
            tmp->type=a->type;
            tmp->value.float_value.val=0-a->value.float_value.val;
            return tmp;
        }
        break;
        case C_INT:{
            Tao_value *tmp = malloc(sizeof(Tao_value));
            tmp->type=a->type;
            tmp->value.int_value.val=0-a->value.int_value.val;
            return tmp;
        }
        break;
    }
    return NULL;
}

Tao_value *biger_two_value(Tao_value *a,Tao_value *b)
{
    switch(a->type)
    {
        case C_FLOAT:{
            Tao_value *tmp = malloc(sizeof(Tao_value));
            tmp->type=C_BOOL;
            tmp->value.bool_value.val=a->value.float_value.val > b->value.float_value.val?1:0;
            return tmp;
        }
        break;
        case C_INT:{
            Tao_value *tmp = malloc(sizeof(Tao_value));
            tmp->type=C_BOOL;
            tmp->value.bool_value.val=a->value.int_value.val > b->value.int_value.val?1:0;
            return tmp;
        }
        break;
    }
    return NULL;
}


Tao_value *bigereq_two_value(Tao_value *a,Tao_value *b)
{
    switch(a->type)
    {
        case C_FLOAT:{
            Tao_value *tmp = malloc(sizeof(Tao_value));
            tmp->type=C_BOOL;
            tmp->value.bool_value.val=a->value.float_value.val >= b->value.float_value.val?1:0;
            return tmp;
        }
        break;
        case C_INT:{
            Tao_value *tmp = malloc(sizeof(Tao_value));
            tmp->type=C_BOOL;
            tmp->value.bool_value.val=a->value.int_value.val >= b->value.int_value.val?1:0;
            return tmp;
        }
        break;
    }
    return NULL;
}

Tao_value *smaller_two_value(Tao_value *a,Tao_value *b)
{
    switch(a->type)
    {
        case C_FLOAT:{
            Tao_value *tmp = malloc(sizeof(Tao_value));
            tmp->type=C_BOOL;
            tmp->value.bool_value.val=a->value.float_value.val < b->value.float_value.val?1:0;
            return tmp;
        }
        break;
        case C_INT:{
            Tao_value *tmp = malloc(sizeof(Tao_value));
            tmp->type=C_BOOL;
            tmp->value.bool_value.val=a->value.int_value.val < b->value.int_value.val?1:0;
            return tmp;
        }
        break;
    }
    return NULL;
}

Tao_value *smallereq_two_value(Tao_value *a,Tao_value *b)
{
    switch(a->type)
    {
        case C_FLOAT:{
            Tao_value *tmp = malloc(sizeof(Tao_value));
            tmp->type=C_BOOL;
            tmp->value.bool_value.val=a->value.float_value.val <= b->value.float_value.val?1:0;
            return tmp;
        }
        break;
        case C_INT:{
            Tao_value *tmp = malloc(sizeof(Tao_value));
            tmp->type=C_BOOL;
            tmp->value.bool_value.val=a->value.int_value.val <= b->value.int_value.val?1:0;
            return tmp;
        }
        break;
    }
    return NULL;
}

Tao_value *equal_two_value(Tao_value *a,Tao_value *b)
{
    switch(a->type)
    {
        case C_FLOAT:{
            Tao_value *tmp = malloc(sizeof(Tao_value));
            tmp->type=C_BOOL;
            tmp->value.bool_value.val=a->value.float_value.val == b->value.float_value.val?1:0;
            return tmp;
        }
        break;
        case C_INT:{
            Tao_value *tmp = malloc(sizeof(Tao_value));
            tmp->type=C_BOOL;
            tmp->value.bool_value.val=a->value.int_value.val == b->value.int_value.val?1:0;
            return tmp;
        }
        break;
        case C_BOOL:{
            Tao_value *tmp = malloc(sizeof(Tao_value));
            tmp->type=C_BOOL;
            tmp->value.bool_value.val=a->value.bool_value.val == b->value.bool_value.val?1:0;
            return tmp;
        }
        break;
    }
    return NULL;
}

Tao_value *noteq_two_value(Tao_value *a,Tao_value *b)
{
    switch(a->type)
    {
        case C_FLOAT:{
            Tao_value *tmp = malloc(sizeof(Tao_value));
            tmp->type=C_BOOL;
            tmp->value.bool_value.val=a->value.float_value.val != b->value.float_value.val?1:0;
            return tmp;
        }
        break;
        case C_INT:{
            Tao_value *tmp = malloc(sizeof(Tao_value));
            tmp->type=C_BOOL;
            tmp->value.bool_value.val=a->value.int_value.val != b->value.int_value.val?1:0;
            return tmp;
        }
        break;
    }
    return NULL;
}

Tao_value *and_two_value(Tao_value *a,Tao_value *b)
{
    switch(a->type)
    {
        case C_BOOL:{
            Tao_value *tmp = malloc(sizeof(Tao_value));
            tmp->type=C_BOOL;
            tmp->value.bool_value.val=(a->value.bool_value.val && b->value.bool_value.val)?1:0;
            return tmp;
        }
        break;
    }
    return NULL;
}

Tao_value *or_two_value(Tao_value *a,Tao_value *b)
{
    switch(a->type)
    {
        case C_BOOL:{
            Tao_value *tmp = malloc(sizeof(Tao_value));
            tmp->type=C_BOOL;
            tmp->value.bool_value.val=(a->value.bool_value.val || b->value.bool_value.val)?1:0;
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
exec_result cal_exp(AST *ast,exec_env *env)
{
    exec_result tar;
    switch(ast->type)
    {
        case A_FUNCALL:{
            tar = exec_funcall(ast,env);
        }
        break;
        case A_IDEN:{
            symbol_list *tmp = symbol_table_find(env->env_symbol_table,ast->content);
            if(tmp==NULL)
            {
                char mess[1000];
                sprintf(mess,"\"%s\" undefined",ast->content);
                exec_error(mess,ast,1);
            }else
            {
                Tao_value *obj = tmp->obj;
                tar.return_value=obj;
                tar.result=R_NOR;
            }
        }
        break;
        case A_TRUE:{
            Tao_value *obj = new_bool(1);
            tar.return_value=obj;
            tar.result=R_NOR;
        }
        case A_FALSE:{
            Tao_value *obj = new_bool(0);
            tar.return_value=obj;
            tar.result=R_NOR;
        }
        break;
        case A_INT:{
            Tao_value *obj = new_int(atoi(ast->content));
            tar.return_value=obj;
            tar.result=R_NOR;
        }
        break;
        case A_FLOAT:{
            Tao_value *obj = new_float(atof(ast->content));
            tar.return_value=obj;
            tar.result=R_NOR;
        }
        break;
        case A_STR:{
            Tao_value *obj = new_str(ast->content);
            tar.return_value=obj;
            tar.result=R_NOR;
        }
        break;
        case A_NOT:{
            AST *ast_exp = Taolist_get(AST*,0,ast->child);
            exec_result rs = cal_exp(ast_exp,env);
            Tao_value *rs_val = rs.return_value;
            if(is_notble(rs_val))
            {
                tar.return_value = not_value(rs_val);
                tar.result=R_NOR;
            }else
            {
                exec_error("cannot \"!\" ,type error",ast,1);
            }
        }
        break;
        case A_MINUS:{
            AST *ast_exp = Taolist_get(AST*,0,ast->child);
            exec_result rs = cal_exp(ast_exp,env);
            Tao_value *rs_val = rs.return_value;
            if(is_minusble(rs_val))
            {
                tar.return_value = minus_value(rs_val);
                tar.result=R_NOR;
            }else
            {
                exec_error("cannot \"-\" ,type error",ast,1);
            }
        }
        break;
        #define make_left_right_ast \
        AST *ast_left = Taolist_get(AST*,0,ast->child);\
        AST *ast_right = Taolist_get(AST*,1,ast->child);\
        exec_result left = cal_exp(ast_left,env);\
        exec_result right = cal_exp(ast_right,env);\
        Tao_value *left_val = left.return_value;\
        Tao_value *right_val = right.return_value;\

        case A_ADD:{
            make_left_right_ast
            if(is_addble(left_val,right_val))
            {
                tar.return_value = add_two_value(left_val,right_val);
                tar.result=R_NOR;
            }else
            {
                exec_error("cannot \"+\" ,type error",ast,1);
            }
        }
        break;
        case A_RED:{
            make_left_right_ast
            if(is_redble(left_val,right_val))
            {
                tar.return_value = red_two_value(left_val,right_val);
                tar.result=R_NOR;
            }else
            {
                exec_error("cannot \"-\" ,type error",ast,1);
            }
        }
        break;
        case A_MUL:{
            make_left_right_ast
            if(is_mulble(left_val,right_val))
            {
                tar.return_value = mul_two_value(left_val,right_val);
                tar.result=R_NOR;
            }else
            {
                exec_error("cannot \"*\" ,type error",ast,1);
            }
        }
        break;
        case A_DIV:{
            make_left_right_ast
            if(is_divble(left_val,right_val))
            {
                tar.return_value = div_two_value(left_val,right_val);
                tar.result=R_NOR;
            }else
            {
                exec_error("cannot \"/\" ,type error",ast,1);
            }
        }
        break;
        case A_MOD:{
            make_left_right_ast
            if(is_modble(left_val,right_val))
            {
                tar.return_value = mod_two_value(left_val,right_val);
                tar.result=R_NOR;
            }else
            {
                exec_error("cannot \"%\" ,type error",ast,1);
            }
        }
        break;
        case A_BIGER:{
            make_left_right_ast
            if(is_bigerble(left_val,right_val))
            {
                tar.return_value = biger_two_value(left_val,right_val);
                tar.result=R_NOR;
            }else
            {
                exec_error("cannot \">\" ,type error",ast,1);
            }
        }
        break;
        case A_BIGEREQ:{
            make_left_right_ast
            if(is_bigereqble(left_val,right_val))
            {
                tar.return_value = bigereq_two_value(left_val,right_val);
                tar.result=R_NOR;
            }else
            {
                exec_error("cannot \">=\" ,type error",ast,1);
            }
        }
        break;
        case A_SMALLER:{
            make_left_right_ast
            if(is_smallerble(left_val,right_val))
            {
                tar.return_value = smaller_two_value(left_val,right_val);
                tar.result=R_NOR;
            }else
            {
                exec_error("cannot \"<\" ,type error",ast,1);
            }
        }
        break;
        case A_SMALLEREQ:{
            make_left_right_ast
            if(is_smallereqble(left_val,right_val))
            {
                tar.return_value = smallereq_two_value(left_val,right_val);
                tar.result=R_NOR;
            }else
            {
                exec_error("cannot \"<=\" ,type error",ast,1);
            }
        }
        break;
        case A_EQUAL:{
            make_left_right_ast
            if(is_equalble(left_val,right_val))
            {
                tar.return_value = equal_two_value(left_val,right_val);
                tar.result=R_NOR;
            }else
            {
                exec_error("cannot \"==\" ,type error",ast,1);
            }
        }
        break;
        case A_NOTEQ:{
            make_left_right_ast
            if(is_noteqble(left_val,right_val))
            {
                tar.return_value = noteq_two_value(left_val,right_val);
                tar.result=R_NOR;
            }else
            {
                exec_error("cannot \"!=\" ,type error",ast,1);
            }
        }
        break;
        case A_AND:{
            make_left_right_ast
            if(is_andble(left_val,right_val))
            {
                tar.return_value = and_two_value(left_val,right_val);
                tar.result=R_NOR;
            }else
            {
                exec_error("cannot \"and\" ,type error",ast,1);
            }
        }
        break;
        case A_OR:{
            make_left_right_ast
            if(is_orble(left_val,right_val))
            {
                tar.return_value = or_two_value(left_val,right_val);
                tar.result=R_NOR;
            }else
            {
                exec_error("cannot \"or\" ,type error",ast,1);
            }
        }
        break;
    }
    return tar;
}

exec_result exec_funcall(AST *ast,exec_env *env)
{
    //执行函数调用
    exec_result tar;
    sysfun func = sysfun_find(env->env_sysfun_list,ast->content);
    AST *args_ast = Taolist_get(AST*,0,ast->child);
    obj_list *args = symbol_list_init();

    for(int i=args_ast->child->len-1;i>=0;i--)
    {
        AST *arg_ast = Taolist_get(AST*,i,args_ast->child);
        exec_result rs = cal_exp(arg_ast,env);
        symbol_list_add(&(args),arg_ast->content,rs.return_value);
    }
    tar.return_value = func(args);
    tar.result=R_NOR;
    return tar;//todo return result
}

void exec_let(AST *ast,exec_env *env)
{
    AST *left = Taolist_get(AST*,0,ast->child);
    AST *right = Taolist_get(AST*,1,ast->child);
    symbol_list *has = symbol_list_find(env->env_symbol_table->head,left->content);
    if(has==NULL)
    {
        symbol_list_add(&(env->env_symbol_table->head),left->content,cal_exp(right,env).return_value);
    }else
    {
        //错误提示：重复定义
        char mess[1000];
        sprintf(mess,"\"%s\" is redefined",left->content);
        exec_error(mess,ast,1);
    }
}

void exec_assign(AST *ast,exec_env *env)
{
    AST *left =Taolist_get(AST*,0,ast->child);
    AST *right =Taolist_get(AST*,1,ast->child);
    symbol_list *tmp = symbol_table_find(env->env_symbol_table,left->content);

    if(tmp==NULL)
    {
        char mess[1000];
        sprintf(mess,"\"%s\" undefined",left->content);
        exec_error(mess,ast,1);
    }else
    {
        tmp->obj = cal_exp(right,env).return_value;
    }
}

exec_result exec_if(AST *ast,exec_env *env)
{
    exec_result rs;
    rs.result=R_NOR;
    //if创建一个作用域
    symbol_list *new_space = symbol_list_init();
    symbol_table_add(&env->env_symbol_table,new_space);

    AST *exp =Taolist_get(AST*,0,ast->child);
    AST *if_stmt =Taolist_get(AST*,1,ast->child);
    //todo 错误处理
    Tao_value *exp_rs = cal_exp(exp,env).return_value;
    if(exp_rs->type==C_BOOL)
    {
        if(exp_rs->value.bool_value.val==1)
        {
            rs = exec_stmt(if_stmt,env);
        }
        else
        {
            if(ast->child->len==3)
            {
                AST *else_stmt =Taolist_get(AST*,2,ast->child);
                if(else_stmt->type==A_IF)
                {
                    rs = exec_if(else_stmt,env);
                }else if(else_stmt->type==A_STMT)
                {
                    rs = exec_stmt(else_stmt,env);
                }
            }
        }
    }else
    {
        exec_error("if need a bool value , type error",ast,1);
    }
    //退出作用域
    symbol_table_del_now(&env->env_symbol_table);
    return rs;
}

exec_result exec_while(AST *ast,exec_env *env)
{
    exec_result rs;
    rs.result=R_NOR;

    AST *exp =Taolist_get(AST*,0,ast->child);
    AST *while_stmt =Taolist_get(AST*,1,ast->child);
    //todo 错误处理

    for(;;)
    {
        Tao_value *exp_rs = cal_exp(exp,env).return_value;
        if(exp_rs->type==C_BOOL)
        {
            if(exp_rs->value.bool_value.val==1)
            {
                //新作用域
                symbol_list *new_space = symbol_list_init();
                symbol_table_add(&env->env_symbol_table,new_space);
                rs = exec_stmt(while_stmt,env);
                if(rs.result==R_BRK)
                {
                    //退出作用域
                    symbol_table_del_now(&env->env_symbol_table);
                    break;
                }else
                if(rs.result==R_CTN)
                {
                    //退出作用域
                    symbol_table_del_now(&env->env_symbol_table);
                    continue;
                }
            }
            else
            {
                break;
            }
        }else
        {
            exec_error("\"while-statement\" need a bool value , type error",ast,1);
        }
    }
    return rs;
}

exec_result exec_stmt(AST *ast,exec_env *env)
{
    exec_result rs;
    rs.result=R_NOR;
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
            case A_ASSIGN:
            {
                exec_assign(a_child,env);
            }
            break;
            case A_IF:
            {
                rs = exec_if(a_child,env);
                if(rs.result==R_CTN || rs.result==R_BRK)
                {
                    return rs;
                }
            }
            break;
            case A_WHILE:
            {
                rs = exec_while(a_child,env);
            }
            break;
            case A_BRK:
            {
                rs.result=R_BRK;
                return rs;
            }
            break;
            case A_CTN:
            {
                rs.result=R_CTN;
                return rs;
            }
            break;
            case A_RTN:
            {
                rs.result=R_RTN;
                //todo：计算表达式的值并返回 rs.return_value=...
                return rs;
            }
            break;
            default:;
        }
    }
    rs.result=R_NOR;
    return rs;//todo return result
}
