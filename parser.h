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


typedef enum
{
    A_ERROR = 1,
    A_LET,          // let
    A_IF,           // if
    A_WHILE,        // while
    A_FOR,          // for
    A_BOOL_EXP,     // bool表达式
    A_NUM_EXP,      // 数值表达式
    A_RTN,          // return
    A_CTN,          // continue
    A_BRK,          // break
    A_TRY,          // try-catch
    A_INT,          // int
    A_FLOAT,        // float
    A_ASSIGN,       // =
    A_ADD,          // +
    A_RED,          // -
    A_MUL,          // *
    A_DIV,          // /
    A_MOD,          // %
    A_BIGER,        // >
    A_BIGEREQ,      // >=
    A_SMALLER,      // <
    A_SMALLEREQ,    // <=
    A_EQUAL,        // ==
    A_FUNCALL,      // print();
    A_FUNDEF,       // def
}AST_type;

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


typedef struct
{
    AST_type type;
    char *content;
    Taolist *child;
}AST;


token_type which_keyword(const char *);
void print_token(token *);
file_string * file_into_string(FILE *fp);
long int get_file_size(FILE *fp);
void jmp_useless(file_string *fs);
token get_a_token(file_string *fs);
void add_char_to_str(char **s,char ch);
void print_token_l(Taolist * l);
Taolist * file_to_token_list(const char *fname);
token * get_token(int n,int i,Taolist *l);

#endif // PARSER_H_INCLUDED
