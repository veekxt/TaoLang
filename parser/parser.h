#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include "../datastr/list.h"

typedef enum lang_node_type{
STMT,
EXP,
NUMBER,
OP_ADD,
OP_REDUCE,
OP_MULTIPLY,
OP_DIVIDE,
}lang_node_type;



struct XTtree
{
    int token_type;
    lang_node_type node_type;
    char *token_is;
    XTlist *child;
    int child_num;
};
struct XTtree * do_exp_num(struct token_list *tl);
struct XTtree * do_exp_exp(struct token_list *tl);
struct XTtree * do_stmt_specific(struct token_list *tl);
#endif // PARSER_H_INCLUDED
