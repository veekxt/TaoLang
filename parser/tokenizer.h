#ifndef TOKENIZER_H_INCLUDED
#define TOKENIZER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../datastr/list.h"

#define NUM_KEYWORDS 3
#define I_IF 0
#define I_ELSE 1
#define I_WHILE 2
#define I_STRING 3
#define I_IDEN 4
#define I_NUMBER 5
#define I_LEFT_BIGQ 6
#define I_RIGHT_BIGQ 7
#define I_LEFT_SMALLQ 8
#define I_RIGHT_SMALLQ 9
#define I_RIGHT_K 10
#define I_LEFT_K 11
#define I_BIGER_EQAUL 12
#define I_SMALLER_EQAUL 13
#define I_NOT_EQAUL 14
#define I_END_LINE_F 15
#define I_ADD 16
#define I_REDUCE 17
#define I_MULTIPLY 18
#define I_DIVIDE 19
#define I_EQAUL 20
#define I_EQAUL_VAR 21
#define I_COMMA 22
#define I_DEF 23
#define I_RETURN 24
#define I_UNDEF 25
#define T_FUN 26
#define I_FLOAT 27
#define I_LET 28
#define I_MOD 29
#define I_END_LINE_N 30
#define I_TRUE 31
#define I_FALSE 32

struct token
{
    int type;
    char *is;
};

struct token_list
{
    int n;//note：int 是否合适？
    XTlist *t;
};
extern char *type_print[100];
struct token_list * init_token_list(void);
int file_to_token_to_array(const char *file_name,struct token_list *t);
void print_token(struct token t);
struct token *token_list_get(struct token_list *,int,int);
#endif // TOKENIZER_H_INCLUDED
