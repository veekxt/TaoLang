#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include "../datastr/list.h"
#include "tokenizer.h"

enum parser_node_type{
STMT,
STRING,
IF_STMT,
WHILE_STMT,
LOOP_BREAK,
LOOP_CONTINUE,
FUN_RETURN,
LET_STMT,
ASSIGN_STMT,
FUNCTION_EXP,
NUM_EXP,
BOOL_EXP,
NUMBER,
IDEN,
OP_ADD,
OP_REDUCE,
OP_MULTIPLY,
OP_DIVIDE,
OP_MOD,
SMALLER_COM,
BIGGER_COM,
SMALLER_COM_EQ,
BIGER_COM_EQ,
EQAUL,
UNKNOWN,
};

struct XTtree
{
    enum parser_node_type tree_type;
    XTlist *child;
    char *content;
};

struct XTtree *init_XTtree(int n);
void print_XTtree_V(struct XTtree *t,int where_i);
void print_XTtree(struct XTtree *);

struct XTtree * do_exp_num(struct token_list *tl);
struct XTtree * do_exp_pri(struct token_list *tl);
struct XTtree * do_exp_exp(struct token_list *tl);
struct XTtree * do_stmt_specific(struct token_list *tl);
struct XTtree * do_stmt(struct token_list *tl);
struct XTtree * do_while(struct token_list *tl);
struct XTtree * do_function(struct token_list *tl);
struct XTtree * do_all_exp(struct token_list *tl);
struct XTtree * do_bool_exp(struct token_list *tl);
struct XTtree * do_assign_stmt(struct token_list *tl);
struct XTtree * do_if(struct token_list *tl);

#endif // PARSER_H_INCLUDED
