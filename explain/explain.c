#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../parser/parser.h"
#include "../datastr/list.h"
#include "explain.h"
#include "sys_fun.h"

/*
int print_symbol_table(struct XTlist *symbol_table)
{
    for(int i=0;i<symbol_table->len;i++)
    {
        struct XTlist * list_var=XTlist_get(symbol_table,i,struct XTlist *);
        for(int j=0;j<list_var->len;j++)
        {
            printf("level %d, %d rd variable,%s == %d\n",i,j,XTlist_get(list_var,j,struct xt_symbol *)->name,XTlist_get(list_var,j,struct xt_symbol *)->value.u.int_value);
        }
    }
    return 0;
}

*/

struct xt_value find_sys_function(const char *name,struct XTlist *argv)
{
    struct xt_value target;
    if(strcmp("print",name)==0)
    {
       xtlang_sys_print(argv);
    }
    return target;
};

struct xt_value find_iden(const char *name,struct XTlist *symbol_table)
{
    struct xt_value tmp;
    for(int i=symbol_table->len-1;i>=0;i--)
    {
        struct XTlist * list_var=XTlist_get(symbol_table,i,struct XTlist *);
        for(int j=list_var->len-1;j>=0;j--)
        {
            if(strcmp(name,XTlist_get(list_var,j,struct xt_symbol *)->name)==0)
            {
                tmp=XTlist_get(list_var,j,struct xt_symbol *)->value;
                return tmp;
            }
        }
    }
    tmp.type=XT_V_NULL;
    return tmp;
};
/**
布尔表达式的计算
*/
struct xt_value cal_bool_exp(struct XTtree *exp,struct XTlist *symbol_table)
{
    struct xt_value tmp;
    tmp.type=XT_V_BOOL;
    tmp.u.bool_value=0;

    struct XTtree *exp_l = XTlist_get(exp->child,0,struct XTtree *);
    struct XTtree *exp_r = XTlist_get(exp->child,1,struct XTtree *);
    struct xt_value tmp_l=cal_exp(exp_l,symbol_table);
    struct xt_value tmp_r=cal_exp(exp_r,symbol_table);

    switch(exp->tree_type)
    {
    case SMALLER_COM:
        tmp.u.bool_value = tmp_l.u.int_value <  tmp_r.u.int_value ?1:0;
    break;
    case BIGGER_COM:
        tmp.u.bool_value = tmp_l.u.int_value >  tmp_r.u.int_value ?1:0;
    break;
    case SMALLER_COM_EQ:
        tmp.u.bool_value = tmp_l.u.int_value <= tmp_r.u.int_value ?1:0;
    break;
    case BIGER_COM_EQ:
        tmp.u.bool_value = tmp_l.u.int_value >= tmp_r.u.int_value ?1:0;
    break;
    case EQAUL:
        tmp.u.bool_value = tmp_l.u.int_value == tmp_r.u.int_value ?1:0;
    break;
    default:;
    }
    return tmp;
};
/**
表达式的计算，递归
*/
struct xt_value cal_exp(struct XTtree *exp,struct XTlist *symbol_table)
{
    struct xt_value tmp;
    tmp.type=XT_V_INT;
    tmp.u.int_value=0;
    switch(exp->tree_type)
    {
        case OP_ADD:
        case OP_DIVIDE:
        case OP_MULTIPLY:
        case OP_REDUCE:
        case OP_MOD:
            {
                struct XTtree *exp_l = XTlist_get(exp->child,0,struct XTtree *);
                struct XTtree *exp_r = XTlist_get(exp->child,1,struct XTtree *);
                struct xt_value tmp_l=cal_exp(exp_l,symbol_table);
                struct xt_value tmp_r=cal_exp(exp_r,symbol_table);
                tmp.type=XT_V_INT;
                switch(exp->tree_type)
                {
                    case OP_ADD:
                        tmp.u.int_value=tmp_l.u.int_value + tmp_r.u.int_value;
                    break;
                    case OP_REDUCE:
                        tmp.u.int_value=tmp_l.u.int_value - tmp_r.u.int_value;
                    break;
                    case OP_MULTIPLY:
                        tmp.u.int_value=tmp_l.u.int_value * tmp_r.u.int_value;
                    break;
                    case OP_DIVIDE:
                        tmp.u.int_value=tmp_l.u.int_value / tmp_r.u.int_value;
                    break;
                    case OP_MOD:
                        tmp.u.int_value=tmp_l.u.int_value % tmp_r.u.int_value;
                    break;
                }
            }
            break;
        case NUMBER:
            {
                tmp.u.int_value=atoi(exp->content);
                tmp.type=XT_V_INT;
            }
        break;
        case IDEN:
            {
                struct xt_value iden_value = find_iden(exp->content,symbol_table);
                tmp=iden_value;
            }
            break;
        case STRING:
            {
                tmp.u.string_value=exp->content;
                tmp.type=XT_V_STR;
            }
            break;
    }
    return tmp;
};
/**
解释AST，递归
初始符号表栈由外部提供
每遇到新的作用域就建一个符号列表，并进入符号表栈
离开作用域时出栈
*/
int explain(struct XTtree *root,struct XTlist *symbol_table)
{
    struct XTlist *list_var=init_XTlist(0,sizeof(struct xt_symbol *));//当前符号表
    XTlist_add(symbol_table,struct XTlist *,list_var);                  //当前符号表放在栈的最顶层
    //symbol_table->current+=1;                                       // 栈指针+1

    for(int i=0;i<root->child->len;i++)
    {
        struct XTtree *tmp = XTlist_get(root->child,i,struct XTtree *);
        switch(tmp->tree_type)
        {
        //let 声明语句
        case LET_STMT:
            {
                struct xt_symbol *var = (struct xt_symbol *)malloc(sizeof(struct xt_symbol));
                var->name=(XTlist_get(tmp->child,0,struct XTtree *))->content;
                var->value=cal_exp(XTlist_get(tmp->child,1,struct XTtree *),symbol_table);
                XTlist_add(list_var,struct xt_symbol *,var);
            }
            break;
        //语句块，递归调用
        case STMT:
            {
                explain(tmp,symbol_table);
            }
            break;
        //单独的表达式，暂时不用
        case NUM_EXP:
        case OP_ADD:
        case OP_DIVIDE:
        case OP_MULTIPLY:
        case OP_REDUCE:
            {
               // struct xt_value target = cal_exp(tmp,symbol_table);
            }
            break;
        //函数调用
        case FUNCTION_EXP:
            {
                struct XTlist *list_var=init_XTlist(0,sizeof(struct xt_value));
                for(int i=0;i<tmp->child->len;i++)
                {
                    struct XTtree *node_arg_tmp = XTlist_get(tmp->child,i,struct XTtree *);
                    struct xt_value arg_tmp = cal_exp(node_arg_tmp,symbol_table);
                    XTlist_add(list_var,struct xt_value,arg_tmp);
                }
                find_sys_function(tmp->content,list_var);
            }
        break;
        //if语句,根据计算结果判断是否执行内部语句快,如果有else，则分条件执行
        case IF_STMT:
            {
                struct XTtree * bool_exp=XTlist_get(tmp->child,0,struct XTtree *);
                struct XTtree * if_stmt=XTlist_get(tmp->child,1,struct XTtree *);
                struct XTtree * if_stmt_else =NULL;
                if(tmp->child->len == 3 )if_stmt_else = XTlist_get(tmp->child,2,struct XTtree *);

                if(cal_bool_exp(bool_exp,symbol_table).u.bool_value != 0)
                {
                    explain(if_stmt,symbol_table);
                }else
                {
                    if(if_stmt_else != NULL) explain(if_stmt_else,symbol_table);
                }
            }
        break;
        }
    }
    XTlist_pop(symbol_table,struct XTlist *);
    return 0;
}
