#ifndef TOKENIZER_H_INCLUDED
#define TOKENIZER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
#define I_END_LINE 15
#define I_ADD 16
#define I_REDUCE 17
#define I_MULTIPLY 18
#define I_DIVIDE 19
#define I_EQAUL 20
#define I_RIGHT_K_EQAUL 21
#define I_LEFT_K_EQAUL 22
#define I_EQAUL_VAR 23
#define I_COMMA 24
#define I_DEF 25
#define I_RETURN 26

struct token{
int type;
char *is;
};

struct token_list{
    int n;//note：int 是否合适？
    int max_len;
    struct token *t;
};

int file_to_token_to_array(const char *file_name,struct token_list *t);

#endif // TOKENIZER_H_INCLUDED
