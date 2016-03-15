#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include <string.h>
#include "list.h"

#define is_white(ch) (((ch)=='\t' || (ch)==' ' || (ch)=='\r' || (ch)=='\f')?1:0)

//for tokenizer.c && parser.c

typedef enum
{
    T_ERROR =1,
    T_INT,          // 89
    T_FLOAT,        // 3.14
    T_ASSIGN,       // =
    T_VISIT,        // .
    T_SVISIT,       // :
    T_STRING,       // "
    T_COMMA,        // ,
    T_SEMI,         // ;
    T_SEMI_N,       // \n
    T_LPAR,         // (
    T_RPAR,         // )
    T_LSQB,         // [
    T_RSQB,         // ]
    T_LBRACE,       // {
    T_RBRACE,       // }
    T_ADD,          // +
    T_MINUS,        // -
    T_STAR,         // *
    T_SLASH,        // /
    T_PERSENT,      // %
    T_TWOSTAR,      // **
    T_LESS,         // <
    T_MORE,         // >
    T_LESSEQ,       // <=
    T_MOREEQ,       // >=
    T_EQUAL,        // ==
    T_NOTEQUAL,     // !=
    T_TO,           // =>
    T_OR,           // ||
    T_AND,          // &&
    T_NOT,          // !
    T_LAMBDA,       // lambda
    T_NULL,         // null
    T_LET,          // let
    T_TRUE,         // true
    T_FALSE,        // false
    T_DEF,          // def
    T_CLASS,        // class
    T_CTN,          // continue
    T_BRK,          // break
    T_TRY,          // try
    T_CATCH,        // catch
    T_THROW,        // throw
    T_IF,           // if
    T_ELIF,         // elif
    T_ELSE,         // else
    T_WHILE,        // while
    T_FOR,          // for
    T_DO,           // do
    T_IN,           // in
    T_IDEN,         // variabel
    T_END,          // file end
} token_type;

typedef struct
{
    token_type type;
    int line;
    char *content;
} token;

typedef struct
{
    long int len;
    char *cur;
    long int line;
    char str[0];
} file_string;

token_type which_keyword(const char *);
void print_token(token *);
file_string * file_into_string(FILE *fp);
long int get_file_size(FILE *fp);
void jmp_useless(file_string *fs);
token get_a_token(file_string *fs);
void add_char_to_str(char **s,char ch);
void print_token_l(Taolist * l);
Taolist * file_to_token_list(const char *fname);

#endif // PARSER_H_INCLUDED
