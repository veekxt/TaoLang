#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "list.h"
#define match_n(t,n) (t)->cur+=(n);

int match(token_type type,Taolist *l)
{
    if(get_token(0,0,l)->type == type)
    {
        l->cur++;
        return 1;
    }
    //todo:错误处理
    return 0;
}

int make_ast_type(token *t,AST *a)
{
    switch(t->type)
    {
        case T_ADD:     a->type = A_ADD;break;
        case T_MINUS:   a->type = A_RED;break;
        case T_STAR:    a->type = A_MUL;break;
        case T_SLASH:   a->type = A_DIV;break;
        case T_PERSENT: a->type = A_MOD;break;
        case T_NOT:     a->type = A_NOT;break;
        case T_VISIT:   a->type = A_VISIT;break;
        case T_INT:     a->type = A_INT;break;
        case T_IDEN:    a->type = A_IDEN;break;
        case T_LESS:    a->type = A_SMALLER;break;
        case T_LESSEQ:    a->type = A_SMALLEREQ;break;
        case T_MORE:    a->type = A_BIGER;break;
        case T_MOREEQ:    a->type = A_BIGEREQ;break;
        case T_EQUAL:    a->type = A_EQUAL;break;
        case T_NOTEQUAL:    a->type = A_NOTEQ;break;
        case T_AND:    a->type = A_AND;break;
        case T_OR:    a->type = A_OR;break;
        default:return 0;
    }
    return 1;
}

void AST_print(AST *t,int where_i)
{
    if(t==NULL)return;

    char *AST_comment[100]=
    {
        "",
        "nil",
        "error",
        "let",
        "if",
        "while",
        "for",
        "exp",
        "return",
        "continue",
        "break",
        "try",
        "int",
        "float",
        "iden",
        "visit.",
        "=",
        "+",
        "-",
        "(-)",
        "*",
        "/",
        "%",
        ">",
        ">=",
        "<",
        "<=",
        "!=",
        "not",
        "==",
        "||",
        "&&",
        "func",
        "def",
        "args",
    };

    if(t->content==NULL || strlen(t->content)==0)
    {
        printf("%s",AST_comment[t->type]);
    }else
    {
        printf("%s:%s",AST_comment[t->type],t->content);
    }
    if(t->child->data!=NULL)
    {

        for(int i=0;i<t->child->len;i++)
        {
            printf("\n");
            for(int i=0;i<=where_i;i++)
            {
                printf("  ");
            }
            AST_print(Taolist_get(AST*,i,t->child),where_i+1);
        }
    }
}

/***

表达式，带优先级
和 build_* 函数相对应
这样虽然会允许一些不合语义语法，但是语义分析阶段可以处理


exp ::=         bool_exp {op8 bool_exp}               // 逻辑与或
bool_exp ::=    com_exp  {op7 com_exp}         //比较运算
com_exp ::=     num_exp  {op6 num_exp}      //加或减
num_exp ::=     sin_exp{op5 sin_exp}   //乘或除
sin_exp ::=   [ op4 ] top_call_exp               //单目运算
top_call_exp ::= start_call_exp {OP3 call_exp}   //最高优先级的运算，call_exp

start_call_exp ::=  call_exp
                | "(" exp ")"                   //括号优先最高
                | num                           //数字字面量
                | float
                | string                        //字符串字面量

call_exp ::= iden                //属性调用
            | fun              //最简函数调用，如 print(1,2,3)

fun ::= iden "(" argv ")"  //最简函数调用，如 print(1,2,3)
argv ::= exp {"," exp}           //参数列表

*/

AST * AST_init(int n)
{
    AST *ast = (AST *)malloc(sizeof(AST));
    ast->type=A_NIL;
    ast->content="";
    ast->child = Taolist_init(AST *,n);
    return ast;
}

AST * build_exp(Taolist *t)
{
    //puts("build_bool_exp");
    AST *s_num_exp = build_bool_exp(t);
    AST *root = s_num_exp;
    while(1)
    {
        token *cur = get_token(0,0,t);
        if(cur->pri == OP8)
        {
            match_n(t,1);
            AST * now = AST_init(2);
            make_ast_type(cur,now);
            Taolist_add(AST *,now->child,root);
            Taolist_add(AST *,now->child,build_bool_exp(t));
            root = now;
        }else
        {
            break;
        }
    }
    return root;
}

AST * build_bool_exp(Taolist *t)
{
    //puts("build_bool_exp");
    AST *s_num_exp = build_com_exp(t);
    AST *root = s_num_exp;
    while(1)
    {
        token *cur = get_token(0,0,t);
        if(cur->pri == OP7)
        {
            match_n(t,1);
            AST * now = AST_init(2);
            make_ast_type(cur,now);
            Taolist_add(AST *,now->child,root);
            Taolist_add(AST *,now->child,build_com_exp(t));
            root = now;
        }else
        {
            break;
        }
    }
    return root;
}


AST * build_com_exp(Taolist *t)
{
    //puts("build_com_exp");
    AST *s_num_exp = build_num_exp(t);
    AST *root = s_num_exp;
    while(1)
    {
        token *cur = get_token(0,0,t);
        if(cur->pri == OP6)
        {
            match_n(t,1);
            AST * now = AST_init(2);
            make_ast_type(cur,now);
            Taolist_add(AST *,now->child,root);
            Taolist_add(AST *,now->child,build_num_exp(t));
            root = now;
        }else
        {
            break;
        }
    }
    return root;
}

AST * build_num_exp(Taolist *t)
{
    //puts("build_num_exp");
    AST *s_sin_exp = build_sin_exp(t);
    AST *root = s_sin_exp;
    while(1)
    {
        token *cur = get_token(0,0,t);
        if(cur->pri == OP5)
        {
            match_n(t,1);
            AST * now = AST_init(2);
            make_ast_type(cur,now);
            Taolist_add(AST *,now->child,root);
            Taolist_add(AST *,now->child,build_sin_exp(t));
            root = now;
        }else
        {
            break;
        }
    }
    return root;
}

AST * build_sin_exp(Taolist *t)
{
    //puts("build_sin_exp");
    token *cur = get_token(0,0,t);
    if(cur->type==T_MINUS || cur->pri == OP4)
    {
        match_n(t,1);
        AST * root = AST_init(1);
        root->type = cur->type==T_NOT?A_NOT:A_MINUS;
        Taolist_add(AST *,root->child,build_top_exp(t));
        return root;
    }else
    {
        return build_top_exp(t);
    }
}

AST * build_top_exp(Taolist *t)
{
    //puts("build_top_exp");
    AST *s_s_exp = build_start_call_exp(t);
    AST *root = s_s_exp;
    while(1)
    {
        token *cur = get_token(0,0,t);
        if(cur->pri == OP3)
        {
            match_n(t,1);
            AST * now = AST_init(2);
            make_ast_type(cur,now);
            Taolist_add(AST *,now->child,root);
            Taolist_add(AST *,now->child,build_call_exp(t));
            root = now;
        }else
        {
            break;
        }
    }
    return root;
}

AST * build_start_call_exp(Taolist *t)
{
    //puts("build_start_call_exp");
    AST *root=NULL;
    token *cur = get_token(0,0,t);
    switch(cur->type)
    {
        case T_IDEN:
        {
            root = build_call_exp(t);
        }
        break;
        case T_INT:
        {
            match_n(t,1);
            root = AST_init(0);
            make_ast_type(cur,root);
            root->content = cur->content;
        }
        break;
        case T_LPAR:
        {
            match_n(t,1);
            root = build_exp(t);
            match(T_RPAR,t);
        }
        break;
        default:;
    }
    return root;
}

AST * build_call_exp(Taolist *t)
{
    //puts("build_call_exp");
    AST *root=NULL;
    token *cur = get_token(0,0,t);
    token *next = get_token(1,0,t);
    switch(next->type)
    {
        case T_LPAR:
        {
            root = build_fun_exp(t);
        }
        break;
        default:
        {   match_n(t,1);
            root = AST_init(0);
            make_ast_type(cur,root);
            root->content=cur->content;
        }
        break;
    }
    return root;
}

AST * build_fun_exp(Taolist *t)
{
    //puts("build_fun_exp");
    AST *root = AST_init(0);
    root->type = A_FUNCALL;
    token *cur = get_token(0,0,t);
    root->content = cur->content;
    match_n(t,2);
    Taolist_add(AST*,root->child,build_argv_exp(t));
    cur = get_token(0,0,t);

    switch(cur->type)
    {
        case T_RPAR:
        {
            match_n(t,1);
        }
        break;
        default:
        {
            //错误：缺少右括号
        }
        break;
    }
    return root;
}

AST * build_argv_exp(Taolist *t)
{
    //puts("build_argv_exp");
    AST *root = AST_init(0);
    root ->type = A_ARGV;
    Taolist_add(AST *,root->child,build_exp(t));
    while(1)
    {
        token *cur = get_token(0,0,t);
        if(cur->type == T_COMMA)
        {
            match_n(t,1);
            Taolist_add(AST *,root->child,build_exp(t));
        }else
        {
            break;
        }
    }
    return root;
}
