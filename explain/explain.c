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
            printf("在第%d层的第%d个变量,%s\n",i,j,XTlist_get(list_var,j,struct xt_symbol *)->name);
        }
    }
    return 0;
}

int explain(struct XTtree *root,struct XTlist *symbol_table)
{
    struct XTlist *list_var=init_XTlist(0,sizeof(struct xt_symbol *));//顶层符号表
    XTlist_add(symbol_table,struct XTlist *,list_var);                  //顶层符号表放在栈的最顶层
    symbol_table->current+=1;                                       // 栈指针+1
    struct XTlist *list_function=init_XTlist(0,sizeof(struct xt_function *));//函数表

    for(int i=0;i<root->child->len;i++)
    {
        struct XTtree *tmp = XTlist_get(root->child,i,struct XTtree *);
        switch(tmp->tree_type)
        {
        case ASSIGN_STMT:
            {
                struct xt_symbol *var_name = (struct xt_symbol *)malloc(sizeof(struct xt_symbol));
                var_name->name=(XTlist_get(tmp->child,0,struct XTtree *))->content;
                XTlist_add(list_var,struct xt_symbol *,var_name);
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
