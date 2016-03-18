#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED
#include <string.h>
#include "list.h"

#define is_white(ch) (((ch)=='\t' || (ch)==' ' || (ch)=='\r' || (ch)=='\f')?1:0)

//for tokenizer.c && parser.c

typedef enum
{
    OP0,
    OP1=10,
    OP2,
    OP3,
    OP4,
    OP5,
    OP6,
    OP7,
    OP8,
    OP9,
    OP10,
}priority;

typedef enum
{
    IN_ROOT=1,
    IN_WHILE,
    IN_IF,
    IN_FOR,
}stmt_place;

typedef struct
{
    int IS_RIGHT;
}parser_status;

extern parser_status PARSER_STATUS;

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
    A_NIL = 1,
    A_ERROR,
    A_STMT,
    A_LET,          // let
    A_IF,           // if
    A_WHILE,        // while
    A_FOR,          // for
    //A_BOOL_EXP,     // bool表达式
    A_EXP,      // 数值表达式
    A_RTN,          // return
    A_CTN,          // continue
    A_BRK,          // break
    A_TRY,          // try-catch
    A_INT,          // int
    A_FLOAT,        // float
    A_STR,        //
    A_IDEN,
    A_VISIT,
    A_ASSIGN,       // =
    A_ADD,          // +
    A_RED,          // -
    A_MINUS,
    A_TWOSTAR,
    A_MUL,          // *
    A_DIV,          // /
    A_MOD,          // %
    A_BIGER,        // >
    A_BIGEREQ,      // >=
    A_SMALLER,      // <
    A_SMALLEREQ,    // <=
    A_NOTEQ,
    A_NOT,
    A_EQUAL,        // ==
    A_OR,        // or
    A_AND,        // and
    A_FUNCALL,      // print();
    A_FUNDEF,       // def
    A_ARGV,
    A_END,
}AST_type;

extern char *AST_comment[];
extern char *token_name[];

typedef struct
{
    token_type type;//token类型
    priority pri;  //优先级或者分类，语法分析会用到
    int line;       //行号，错误提示
    char *content;  //内容
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

void AST_print(AST *t,int where_i,int);
void AST_free(AST *a);
void goto_next_stmt(Taolist *t);

AST * build_root_stmt(Taolist *t,int);
AST * build_stmt(Taolist *t);
AST * build_let_stmt(Taolist *t);
AST * build_if_stmt(Taolist *t);
AST * build_while_stmt(Taolist *t);
AST * build_assign_stmt(Taolist *t);
AST * build_assign_stmt_1(Taolist *t);
AST * build_assign_stmt_2(Taolist *t,token *cur,token *next);

AST * build_iden_exp(Taolist *);
AST * build_exp(Taolist *t);
AST * build_bool_exp(Taolist *t);
AST * build_com_exp(Taolist *t);
AST * build_num_exp(Taolist *t);
AST * build_sin_exp(Taolist *t);
AST * build_top_exp(Taolist *t);
AST * build_start_call_exp(Taolist *t);
AST * build_call_exp(Taolist *t);
AST * build_fun_exp(Taolist *t);
AST * build_argv_exp(Taolist *t);
#endif // PARSER_H_INCLUDED
