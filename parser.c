#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "list.h"
#include "parser_comment.h"
/**
宏，为一个父节点添加一个子节点
使用return方便错误处理
*/
#define add_child_node(father,son)\
do{\
    AST *not_u_tmp = son;\
    if(not_u_tmp == NULL){AST_free(father);return NULL;}\
    Taolist_add(AST *,father->child,not_u_tmp);\
}while(0);

#define match(t_type,l,ast)\
do{\
    token *cur = get_token(0,0,l);\
    if(cur->type == t_type)\
    {\
        l->cur++;\
    }else\
    {\
        syntax_error_unex(cur);\
        AST_free(ast);\
        return NULL;\
    }\
}while(0);

#define match_n(t,n) (t)->cur+=(n);

#define in_block(where) (where==A_IF || where==A_WHILE || where==A_FUNDEF)

#define in_function(where) (where==A_FUNDEF)

//并没有什么卵用，如果while里嵌入了if ，依然是错误
//#define in_loop(where) (where==A_WHILE || where==A_FOR)

parser_status PARSER_STATUS=
{
    1,
};

void AST_free(AST *a)
{
    Taolist_kill(a->child);
    free(a);
}

int syntax_error_unex(token *cur)
{
    printf("line %d ,syntax error :unexpected token (%s) => %s\n",\
           cur->line,token_name[cur->type],cur->content==NULL?"":cur->content);
    //exit(0);
    PARSER_STATUS.IS_RIGHT =0;
    return 0;
}

int make_ast_type(token *t,AST *a)
{
    a->line=t->line;
    switch(t->type)
    {
    case T_ADD:
        a->type = A_ADD;
        break;
    case T_MINUS:
        a->type = A_RED;
        break;
    case T_STAR:
        a->type = A_MUL;
        break;
    case T_SLASH:
        a->type = A_DIV;
        break;
    case T_PERSENT:
        a->type = A_MOD;
        break;
    case T_NOT:
        a->type = A_NOT;
        break;
    case T_VISIT:
        a->type = A_VISIT;
        break;
    case T_INT:
        a->type = A_INT;
        break;
    case T_FLOAT:
        a->type = A_FLOAT;
        break;
    case T_STRING:
        a->type = A_STR;
        break;
    case T_IDEN:
        a->type = A_IDEN;
        break;
    case T_LESS:
        a->type = A_SMALLER;
        break;
    case T_LESSEQ:
        a->type = A_SMALLEREQ;
        break;
    case T_MORE:
        a->type = A_BIGER;
        break;
    case T_MOREEQ:
        a->type = A_BIGEREQ;
        break;
    case T_EQUAL:
        a->type = A_EQUAL;
        break;
    case T_NOTEQUAL:
        a->type = A_NOTEQ;
        break;
    case T_AND:
        a->type = A_AND;
        break;
    case T_OR:
        a->type = A_OR;
        break;
    case T_TWOSTAR:
        a->type = A_TWOSTAR;
        break;
    case T_BRK:
        a->type = A_BRK;
        break;
    case T_CTN:
        a->type = A_CTN;
        break;
    case T_RTN:
        a->type = A_RTN;
        break;
    case T_TRUE:
        a->type = A_TRUE;
        break;
    case T_FALSE:
        a->type = A_FALSE;
        break;
    case T_LET:
        a->type = A_LET;
        break;
    default:
        return 0;
    }
    return 1;
}

void AST_print(AST *t,int where_i,int has_pre)
{
    if(t==NULL)return;
    if(t->content==NULL || strlen(t->content)==0)
    {
        printf("%s",AST_comment[t->type]);
    }
    else
    {
        printf("%s:%s",AST_comment[t->type],t->content);
    }
    if(t->child->data!=NULL)
    {

        for(int i=0; i<t->child->len; i++)
        {
            printf("\n");
            for(int i=0; i<=where_i; i++)
            {
                printf("  ");
            }
            if(i==t->child->len - 1)has_pre=0;
            else has_pre=1;
            AST_print(Taolist_get(AST*,i,t->child),where_i+1,has_pre);
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

let语句
let_stmt ::= "let" iden "=" exp;
赋值语句
assign_stmt ::= iden "=" exp;
            | iden (OP5 | OP6) "=" exp;
if语句
if_stmt ::= "if" exp : stmt {elsif exp: stmt} [else: stmt]
语句
stmt ::= "{" many_stmt "}"
many_stmt ::= sin_stmt{ ;sin_stmt }
sin_stmt ::=   ;
        | exp
        | assign_exp
        | ...
*/

AST * AST_init(int n)
{
    AST *ast = (AST *)malloc(sizeof(AST));
    ast->type=A_NIL;
    ast->line=0;
    ast->content=NULL;
    ast->child = Taolist_init(AST *,n);
    return ast;
}

void del_void_token(Taolist *t)
{
    for(;;)
    {
        token *cur = get_token(0,0,t);
        if(cur->type==T_SEMI_N)
        {
            t->cur++;
        }
        else break;
    }
}

void goto_next_stmt(Taolist *t)
{
    for(;;)
    {
        token *cur = get_token(0,0,t);
        if(cur->type==T_SEMI_N || cur->type==T_END)
        {
            //t->cur++;
            break;
        }
        else t->cur++;
    }
}

AST * build_root_stmt(Taolist *t,AST_type where)
{
    del_void_token(t);
    AST *root = AST_init(0);
    root->type = A_STMT;
    AST *tmp = build_stmt(t,where);
    Taolist_add(AST*,root->child,tmp);
    token *cur=NULL;

    for(;;)
    {
        cur = get_token(0,0,t);
        switch(cur->type)
        {
        case T_SEMI_N:
            match_n(t,1);
            cur = get_token(0,0,t);
            if(in_block(where) && cur->type==T_RBRACE)return root;
            tmp = build_stmt(t,where);
            if(tmp==NULL)
            {
                goto_next_stmt(t);
                continue;
            }
            Taolist_add(AST*,root->child,tmp);
            break;
        case T_END:
            return root;
        case T_RBRACE:
            if(in_block(where)){return root;}//if语句允许分号不换行
        default:
        {
            syntax_error_unex(cur);
            goto_next_stmt(t);
        }
        }
    }
    return root;
}

AST * build_stmt(Taolist *t,AST_type where)
{
    AST *root = NULL;
    token *cur = get_token(0,0,t);
    switch(cur->type)
    {
    case T_END:
    {
        root = AST_init(0);
        root->type=A_END;
        root->line=cur->line;
    }
    break;
    case T_INT:
    case T_FLOAT:
    case T_TRUE:
    case T_FALSE:
    {
        root = build_exp(t);
    }
    break;
    case T_LET:
    {
        root = build_let_stmt(t);
    }
    break;
    case T_IF:
    {
        root = build_if_stmt(t);
    }
    break;
    case T_DEF:
    {
        root = build_fun_def_stmt(t);
    }
    break;
    case T_WHILE:
    {
        root = build_while_stmt(t);
    }
    break;
    case T_RTN:
        {
            if(in_function(where))
            {
                root = AST_init(1);
                make_ast_type(cur,root);
                match_n(t,1);
                token *cur = get_token(0,0,t);
                if(cur->type!=T_SEMI_N)add_child_node(root,build_exp(t));
            }else{
                syntax_error_unex(cur);
            }
        }
    break;
    //todo 如何判断处于while中？不在循环里不应该通过
    case T_CTN:
    case T_BRK:
    {
        root = AST_init(0);
        make_ast_type(cur,root);
        match_n(t,1);
    }
    break;
    case T_IDEN:
    {
        root = build_assign_stmt(t);
    }
    break;
    //case T_SEMI:
    //case T_SEMI_N:
    //return NULL;
    //break;
    default:
    {
        syntax_error_unex(cur);
        return NULL;
    }
    }
    return root;
}

/**
匹配一个if语句，type表示是if elif 还是else。
*/

AST * build_one_if_stmt(Taolist *t,token *cur)
{
    AST *root=  AST_init(3);
    token_type type=cur->type;
    match(type,t,root);

    if(type==T_IF || type==T_ELIF){root->type=A_IF;root->line=cur->line;}
    else {root->type=A_STMT;root->line=cur->line;}
    if(type!=T_ELSE)add_child_node(root,build_exp(t));
    del_void_token(t);
    match(T_LBRACE,t,root);
    AST *tmp= build_root_stmt(t,A_IF);
    add_child_node(root,tmp);

    match(T_RBRACE,t,root);
    if(type==T_ELSE)
    {
        AST_free(root);
        root = tmp;
    }
    return root;

}

AST * build_assign_stmt(Taolist *t)
{
    AST *l_value = build_exp(t);

    token *cur = get_token(0,0,t);
    token *next = get_token(1,0,t);
    //如果
    if(cur->pri<=OP8 && cur->pri>=OP3)
    {
        if(next->type==T_ASSIGN)
        {
            //第二种赋值
            //return build_assign_stmt_2(t,cur,next);
            AST *root=AST_init(2);
            AST *exp=AST_init(2);
            root->type=A_ASSIGN;
            root->line=cur->line;
            make_ast_type(cur,exp);
            add_child_node(exp,l_value);
            match_n(t,2);
            add_child_node(exp,build_exp(t));
            add_child_node(root,l_value);
            add_child_node(root,exp);
            return root;
        }
    }else if(cur->type == T_ASSIGN)
    {
        //return build_assign_stmt_1(t);
        AST *root = AST_init(2);
        root->type=A_ASSIGN;
        root->line=cur->line;
        add_child_node(root,l_value);
        match_n(t,1);
        add_child_node(root,build_exp(t));
        return root;
    }
    return l_value;
}


AST * build_while_stmt(Taolist *t)
{
    token *cur = get_token(0,0,t);
    AST *root=  AST_init(2);
    match(T_WHILE,t,root);

    root->line=cur->line;
    root->type=A_WHILE;

    add_child_node(root,build_exp(t));
    del_void_token(t);
    match(T_LBRACE,t,root);
    AST *tmp= build_root_stmt(t,A_WHILE);
    add_child_node(root,tmp);

    match(T_RBRACE,t,root);
    return root;
}

AST * build_if_stmt(Taolist *t)
{
    token *cur = get_token(0,0,t);
    token *next = get_token(1,0,t);
    AST *root = build_one_if_stmt(t,cur);
    AST *target = root;
    if(root!=NULL)
    {
        for(;;)
        {
            cur = get_token(0,0,t);
            next = get_token(1,0,t);
            if(cur->type == T_ELIF||next->type == T_ELIF)
            {
                del_void_token(t);
                AST *tmp = build_one_if_stmt(t,cur);
                add_child_node(target,tmp);
                target=tmp;
            }else
            {
                break;
            }
        }
    }
    cur = get_token(0,0,t);
    next = get_token(1,0,t);
    if(cur->type == T_ELSE||next->type == T_ELSE)
    {
        del_void_token(t);
        AST *tmp = build_one_if_stmt(t,cur);
        add_child_node(target,tmp);
        target=tmp;
    }
    return root;
}

AST * build_let_stmt(Taolist *t)
{
    AST *root=  AST_init(2);
    token *cur = get_token(0,0,t);
    make_ast_type(cur,root);
    match(T_LET,t,root);
    cur = get_token(0,0,t);
    if(cur->type==T_IDEN)
    {
        add_child_node(root,build_iden_exp(t));
        match(T_ASSIGN,t,root);
        add_child_node(root,build_exp(t));
    }
    else
    {
        syntax_error_unex(cur);
        return NULL;
    }
    return root;
}

AST *build_iden_exp(Taolist *t)
{
    token *cur = get_token(0,0,t);
    if(cur->type==T_IDEN)
    {
        AST *iden=AST_init(0);
        iden->type=A_IDEN;
        iden->line=cur->line;
        iden->content = cur->content;
        match_n(t,1);
        return iden;
    }
    else
    {
        syntax_error_unex(cur);
        return NULL;
    }
}

AST * build_exp(Taolist *t)
{
    //puts("build_bool_exp");
    AST *s_num_exp = build_bool_exp(t);
    AST *root = s_num_exp;
    if(root!=NULL)
    while(1)
    {
        token *cur = get_token(0,0,t);
        if(cur->pri == OP8)
        {
            if(get_token(1,0,t)->type==T_ASSIGN)
            {
                break;
            }
            match_n(t,1);
            AST * now = AST_init(2);
            make_ast_type(cur,now);
            add_child_node(now,root);
            add_child_node(now,build_bool_exp(t));
            root = now;
        }
        else
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
    if(root!=NULL)
    while(1)
    {
        token *cur = get_token(0,0,t);
        if(cur->pri == OP7)
        {
            if(get_token(1,0,t)->type==T_ASSIGN)
            {
                break;
            }
            match_n(t,1);
            AST * now = AST_init(2);
            make_ast_type(cur,now);
            add_child_node(now,root);
            add_child_node(now,build_com_exp(t));
            root = now;
        }
        else
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
    if(root!=NULL)
    while(1)
    {
        token *cur = get_token(0,0,t);
        if(cur->pri == OP6)
        {
            if(get_token(1,0,t)->type==T_ASSIGN)
            {
                break;
            }
            match_n(t,1);
            AST * now = AST_init(2);
            make_ast_type(cur,now);
            add_child_node(now,root);
            add_child_node(now,build_num_exp(t));
            root = now;
        }
        else
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
    if(root!=NULL)
    while(1)
    {
        token *cur = get_token(0,0,t);
        if(cur->pri == OP5)
        {
            if(get_token(1,0,t)->type==T_ASSIGN)
            {
                break;
            }
            match_n(t,1);
            AST * now = AST_init(2);
            make_ast_type(cur,now);
            add_child_node(now,root);
            add_child_node(now,build_sin_exp(t));
            root = now;
        }
        else
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
        root->line = cur->line;
        add_child_node(root,build_top_exp(t));
        return root;
    }
    else
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
            if(get_token(1,0,t)->type==T_ASSIGN)
            {
                break;
            }
            match_n(t,1);
            AST * now = AST_init(2);
            make_ast_type(cur,now);
            add_child_node(now,root);
            add_child_node(now,build_call_exp(t));
            root = now;
        }
        else
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
    case T_FLOAT:
    case T_STRING:
    case T_TRUE:
    case T_FALSE:
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
        match(T_RPAR,t,root);
    }
    break;
    default:
        syntax_error_unex(cur);
        return NULL;
    }
    return root;
}

AST * build_call_exp(Taolist *t)
{
    //puts("build_call_exp");
    AST *root=NULL;
    //token *cur = get_token(0,0,t);
    token *next = get_token(1,0,t);
    switch(next->type)
    {
    case T_LPAR:
    {
        return build_fun_exp(t);
    }
    break;
    default:
    {
        ;
    }
    break;
    }
    root = build_iden_exp(t);
    return root;
}

AST * build_fun_exp(Taolist *t)
{
    //puts("build_fun_exp");
    AST *root = AST_init(0);
    token *cur = get_token(0,0,t);
    match(T_IDEN,t,root);
    match(T_LPAR,t,root);

    root->type = A_FUNCALL;
    root->line = cur->line;
    root->content = cur->content;

    cur = get_token(0,0,t);
    if(cur->type!=T_RPAR)
    {
        add_child_node(root,build_argv_exp(t));
    }
    match(T_RPAR,t,root);
    return root;
}

AST * build_argv_exp(Taolist *t)
{
    //puts("build_argv_exp");
    AST *root = AST_init(0);
    root ->type = A_ARGV;
    root->line=get_token(0,0,t)->line;
    add_child_node(root,build_exp(t));
    while(1)
    {
        token *cur = get_token(0,0,t);
        if(cur->type == T_COMMA)
        {
            match_n(t,1);
            add_child_node(root,build_exp(t));
        }
        else
        {
            break;
        }
    }
    return root;
}

AST *build_fun_args(Taolist *t)
{
    //puts("build_argv_exp");
    AST *root = AST_init(0);
    token *cur = get_token(0,0,t);
    root ->type = A_ARGS;
    root->line=cur->line;
    if(cur->type==T_RPAR)
    {
        return root;
    }
    add_child_node(root,build_iden_exp(t));
    while(1)
    {
        token *cur = get_token(0,0,t);
        if(cur->type == T_COMMA)
        {
            match_n(t,1);
            add_child_node(root,build_iden_exp(t));
        }
        else
        {
            break;
        }
    }
    return root;
}

AST *build_fun_def_stmt(Taolist *t)
{
    AST *root = AST_init(0);
    root->type = A_FUNDEF;
    root->line=get_token(0,0,t)->line;
    match(T_DEF,t,root);
    add_child_node(root,build_iden_exp(t));
    match(T_LPAR,t,root);
    add_child_node(root,build_fun_args(t));
    match(T_RPAR,t,root);
    match(T_LBRACE,t,root);
    add_child_node(root,build_root_stmt(t,A_FUNDEF));
    match(T_RBRACE,t,root);
    return root;
}
