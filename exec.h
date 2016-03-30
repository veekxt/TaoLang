#ifndef EXEC_H_INCLUDED
#define EXEC_H_INCLUDED
#include "type.h"
#include "parser.h"

///sysfun.c
#define is_empty(l) ((l)->next==NULL)

#define list_init_g(type) \
do{\
    type *not_use_tmp = malloc(sizeof(type));\
    not_use_tmp->next=NULL;\
    return not_use_tmp;\
}while(0);

typedef struct arg_list_t
{
    char *name;
    Tao_value *obj;
    struct arg_list_t *next;
}obj_list,symbol_list;

typedef Tao_value * (*sysfun)(obj_list *args);
typedef struct sysfun_list_t
{
    char *name;
    sysfun func;
    struct sysfun_list_t *next;
}sysfun_list;

symbol_list * symbol_list_init(void);
void symbol_list_kill(symbol_list *l);
void symbol_list_add(symbol_list **l,char *name,Tao_value *obj);
sysfun_list * sysfun_list_init(void);
void sysfun_list_add(sysfun_list **l,char *name,sysfun func);
void add_sysfun(sysfun_list **l);
Tao_value *sys_print(obj_list *args);
Tao_value *sys_input(obj_list *args);
Tao_value *sys_int(obj_list *args);
sysfun sysfun_find(sysfun_list *l,char *s);
///exec.c
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
    Tao_value *return_value;
}exec_result;

symbol_table * symbol_table_init(void);
void symbol_table_add(symbol_table **l,symbol_list *s_head);
exec_env *make_init_env(void);
void exec_let(AST *ast,exec_env *status);
exec_result exec_stmt(AST *ast,exec_env *status);
exec_result exec_funcall(AST *ast,exec_env *env);
exec_result cal_exp(AST *ast,exec_env *env);

#endif // EXEC_H_INCLUDED
