#ifndef EXPLAIN_H_INCLUDED
#define EXPLAIN_H_INCLUDED

#include "../parser/parser.h"
#include "../datastr/list.h"

enum xt_value_type
{
    XT_V_INT,
    XT_V_STR,
    XT_V_NULL,
};

struct xt_value
{
    enum xt_value_type type;
    union {
        int int_value;
        double double_value;
        char * string_value;
    } u;
};

struct xt_symbol
{
    char *name;
    struct xt_value value;
};

struct xt_function
{
    char *name;
    struct xt_value *args;
};

int explain(struct XTtree *,struct XTlist *);
struct xt_value cal_exp(struct XTtree *exp,struct XTlist *symbol_table);
struct xt_value find_iden(const char *name,struct XTlist *symbol_table);
#endif // EXPLAIN_H_INCLUDED
