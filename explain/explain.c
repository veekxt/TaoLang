#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../parser/parser.h"
#include "../datastr/list.h"
#include "explain.h"

int print_symbol_table(struct XTlist *symbol_table)
{
    for(int i=0;i<symbol_table->len;i++)
    {
        struct XTlist * list_var=XTlist_get(symbol_table,i,struct XTlist *);
        for(int j=0;j<list_var->len;j++)
        {
            printf("在第%d层的第%d个变量,%s,值为%d\n",i,j,XTlist_get(list_var,j,struct xt_symbol *)->name,XTlist_get(list_var,j,struct xt_symbol *)->value.u.int_value);
        }
    }
    return 0;
}

struct xt_value cal_exp(struct XTtree *exp)
{
    struct xt_value tmp;
    tmp.u.int_value=10;
    return tmp;
};

int explain(struct XTtree *root,struct XTlist *symbol_table)
{
    struct XTlist *list_var=init_XTlist(0,sizeof(struct xt_symbol *));//当前符号表
    XTlist_add(symbol_table,struct XTlist *,list_var);                  //当前符号表放在栈的最顶层
    symbol_table->current+=1;                                       // 栈指针+1

    for(int i=0;i<root->child->len;i++)
    {
        struct XTtree *tmp = XTlist_get(root->child,i,struct XTtree *);
        switch(tmp->tree_type)
        {
        case ASSIGN_STMT:
            {
                struct xt_symbol *var = (struct xt_symbol *)malloc(sizeof(struct xt_symbol));
                var->name=(XTlist_get(tmp->child,0,struct XTtree *))->content;
                var->value=cal_exp(XTlist_get(tmp->child,1,struct XTtree *));
                XTlist_add(list_var,struct xt_symbol *,var);
            }
            break;
        case STMT:
            {
                explain(tmp,symbol_table);
            }
            break;
        default:;
        }
    }
    return 0;
}
