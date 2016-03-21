#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "exec.h"
#include "parser.h"

symbol_table * symbol_table_init(void)
{
    list_init_g(symbol_table *);
}

void symbol_table_add(symbol_table **l,symbol_list *s_head)
{
    symbol_table *head = malloc(sizeof(symbol_table));
    (*head)->next=*l;
    (*head)->head=s_head;
    *l=head;
}

void exec(AST *ast)
{

}
