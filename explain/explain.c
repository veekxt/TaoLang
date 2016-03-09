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
            //case ASSIGN_STMT:
            case LET_STMT:
            {
                struct xt_symbol *var = (struct xt_symbol *)malloc(sizeof(struct xt_symbol));
                var->name=(XTlist_get(tmp->child,0,struct XTtree *))->content;
                var->value=cal_exp(XTlist_get(tmp->child,1,struct XTtree *),symbol_table);
                XTlist_add(list_var,struct xt_symbol *,var);
            }
            break;
        case STMT:
            {
                explain(tmp,symbol_table);
            }
            break;
        case NUM_EXP:
        case OP_ADD:
        case OP_DIVIDE:
        case OP_MULTIPLY:
        case OP_REDUCE:
            {
               // struct xt_value target = cal_exp(tmp,symbol_table);
            }
            break;
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
        }
    }
    XTlist_pop(symbol_table,struct XTlist *);
    return 0;
}
