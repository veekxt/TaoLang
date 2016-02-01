#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include "../datastr/list.h"

enum parser_node_type{
STMT,
IF_STMT,
WHILE_STMT,
ASSIGN_STMT,
FUNCTION_EXP,
NUM_EXP,
BOOL_EXP,
NUMBER,
OP_ADD,
OP_REDUCE,
OP_MULTIPLY,
OP_DIVIDE,
};

struct XTtree
{
    int token_type;
    enum parser_node_type node_type;
    char *token_is;
    XTlist *child;
    int child_num;
};

struct XTtree *init_XTtree(int n);
void print_XTtree_V(struct XTtree *t,int where_i);
void print_XTtree(struct XTtree *);

struct XTtree * do_exp_num(struct token_list *tl);//token流指向读取的数字下一个
struct XTtree * do_exp_exp(struct token_list *tl);//表达式的下一个字符
struct XTtree * do_stmt_specific(struct token_list *tl);//
struct XTtree * do_stmt(struct token_list *tl);
struct XTtree * do_while(struct token_list *tl);
struct XTtree * do_function(struct token_list *tl);
struct XTtree * do_all_exp(struct token_list *tl);
struct XTtree * do_bool_exp(struct token_list *tl);
struct XTtree * do_assign_stmt(struct token_list *tl);
struct XTtree * do_if(struct token_list *tl);

#endif // PARSER_H_INCLUDED
