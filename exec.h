#ifndef EXEC_H_INCLUDED
#define EXEC_H_INCLUDED
#include "object.h"
#include "sysfun.h"
#include "parser.h"

typedef enum
{
    W_ROOT = 1,//全局
    W_WHILE,//while
    W_FOR,//for
    W_FUNDEF,//fun定义
    W_IF,//if
    //...
}exec_where;

typedef enum
{
    R_BRK = 1,
    R_CTN,
    R_RTN,
    R_NOR,
}exec_stmt_result;

typedef struct symbol_table_t
{
    symbol_list *head;
    struct symbol_table_t *next;
}symbol_table;

typedef struct exec_status_t
{
    exec_where where;//处于什么语句中
    symbol_table *env_symbol_table;//符号表
    sysfun_list *env_sysfun_list;//系统函数表
    //...
    //...
}exec_env;

typedef struct
{
    exec_stmt_result result;
    Tao_object *return_value;
}exec_result;

symbol_table * symbol_table_init(void);
void symbol_table_add(symbol_table **l,symbol_list *s_head);
exec_env *make_init_env(void);
exec_result *exec_let(AST *ast,exec_env *status);
exec_result *exec_stmt(AST *ast,exec_env *status);
exec_result *exec_funcall(AST *ast,exec_env *env);
exec_result *cal_exp(AST *ast,exec_env *env);

#endif // EXEC_H_INCLUDED
