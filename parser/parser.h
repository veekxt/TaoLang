#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

struct XTtree
{
    int token_type;
    char *token_is;
    struct XTtree **child;
    int child_num;
};
struct XTtree * do_exp_num(struct token_list *tl);
struct XTtree * do_exp_exp(struct token_list *tl);
#endif // PARSER_H_INCLUDED
