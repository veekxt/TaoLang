#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

struct XTtree{
int token_type;
char *token_is;
struct XTtree **child;
int child_num;
};

#endif // PARSER_H_INCLUDED
