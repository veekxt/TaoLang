#ifndef EXEC_H_INCLUDED
#define EXEC_H_INCLUDED
#include "object.h"
#include "sysfun.h"

typedef struct symbol_table_t
{
    symbol_list *head;
    struct symbol_table_t *next;
}symbol_table;

#endif // EXEC_H_INCLUDED
