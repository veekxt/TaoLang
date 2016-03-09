#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"
#include "parser.h"
#include "tools.h"
#include "../datastr/list.h"

char *parser_node_type_comment[]={
"stmt",
"string",
"if",
"while",
"let",
"assign",
"fun-call",
"num-exp",
"bool-exp",
"n",
"iden",
"+",
"-",
"*",
"/",
"<",
">",
"<=",
">=",
"==",
"unknown",
};

struct XTtree *init_XTtree(int n)
{
    struct XTtree *t;
    t =(struct XTtree *)malloc(sizeof(struct XTtree));
    t->child=init_XTlist(n,sizeof(struct XTtree *));
    t->tree_type=UNKNOWN;
    t->content="";
    return t;
}

//可视化的树状显示,横向
/**
例子
a-b*c =>

-
  a
  *
    b
    c

*/
void print_XTtree_V(struct XTtree *t,int where_i)
{
    if(t==NULL)return;
    if(strlen(t->content)==0)
    {
        printf("%s",parser_node_type_comment[t->tree_type]);
    }else
    {
        printf("%s:%s",parser_node_type_comment[t->tree_type],t->content);
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
            print_XTtree_V(XTlist_get(t->child,i,struct XTtree *),where_i+1);
        }
    }
}
void print_XTtree(struct XTtree *t)
{
    //输出AST，格式为
    //根（子节点1，子节点2）
    //例子 1/23-(1+90) ==> -(/(1,23),+(1,90))
    //数据大时没有可读性
    if(t==NULL)return;
    if(strlen(t->content)==0)
    {
        printf("%s",parser_node_type_comment[t->tree_type]);
    }else
    {
        printf("%s",t->content);
    }
    if(t->child->data!=NULL)
    {
        printf("(");
        print_XTtree(XTlist_get(t->child,0,struct XTtree *));
        for(int i=1;i<t->child->len;i++)
        {
            printf(",");
            print_XTtree(XTlist_get(t->child,i,struct XTtree *));
        }
        printf(")");
    }
}

struct XTtree * do_string_exp(struct token_list *tl)
{
    struct XTtree *root_tree = init_XTtree(0);
    root_tree->tree_type=STRING;
    struct token *a_token = token_list_get(tl,0,0);
    root_tree->content=a_token->is;
    tl->n++;
    return root_tree;
};

struct XTtree * do_assign_stmt(struct token_list *tl)
{
    struct XTtree *root_tree = init_XTtree(0);
    root_tree->tree_type=ASSIGN_STMT;
    XTlist_add(root_tree->child,struct XTtree *,do_exp_exp(tl));
    tl->n++;
    XTlist_add(root_tree->child,struct XTtree *,do_all_exp(tl));
    return root_tree;
};
struct XTtree * do_all_exp(struct token_list *tl)
{
    int tmp_n=tl->n;
    struct token *a_token = token_list_get(tl,0,0);
    switch(a_token->type)
    {
    case I_IDEN:
    case I_NUMBER:
        {
            struct XTtree *tmp_tree = do_exp_exp(tl);
            struct token *a_token = token_list_get(tl,0,0);
            if(a_token!=NULL && is_relation_sign(a_token->type))
            {
                tl->n=tmp_n;
                free(tmp_tree);
                return do_bool_exp(tl);
            }else
            {
                return tmp_tree;//or tl->n=tmp_n; free(tmp_tree); do_exp_exp(tl);
            };
        }
        break;
    case I_STRING:
        {
            return do_string_exp(tl);
        }
        break;
    }
    return NULL;
}

//布尔表达式
struct XTtree * do_bool_exp(struct token_list *tl)
{
    struct XTtree *tmp_root=init_XTtree(2);
    struct XTtree *tmp=do_exp_exp(tl);

    XTlist_add(tmp_root->child,struct XTtree *,tmp);
    struct token *a_token = token_list_get(tl,0,0);
    switch(a_token->type)
    {
    case I_LEFT_K:
        tmp_root->tree_type=SMALLER_COM;
        break;
    case I_RIGHT_K:
        tmp_root->tree_type=BIGGER_COM;
        break;
    case I_SMALLER_EQAUL:
        tmp_root->tree_type=SMALLER_COM_EQ;
        break;
    case I_BIGER_EQAUL:
        tmp_root->tree_type=BIGER_COM_EQ;
        break;
    case I_EQAUL_VAR:
        tmp_root->tree_type=EQAUL;
        break;
    default:break;
     }
    tl->n++;//match > < >= <= ==
    XTlist_add(tmp_root->child,struct XTtree *,do_exp_exp(tl));
    return tmp_root;

};
/**

//简单四则运算的parser测试，带有优先级

S -> fac { op1 fac }    # do_exp_exp()
fac -> N { op2 N }      # do_exp_pri()
N -> num | (S)          # do_exp_num()
num -> float | int
op1 -> + | -
op2 -> * | /

*/
struct XTtree * do_exp_pri(struct token_list *tl)
{
    struct XTtree *start = do_exp_num(tl);
    struct XTtree *tmp_root=start;
    while(1)//注意这里不知道{ op S}的重复次数
    {
        struct token *a_token_2 = token_list_get(tl,0,0);
        if(a_token_2!=NULL)
        {
            enum parser_node_type type=UNKNOWN;
            switch(a_token_2->type)
            {
                case I_MULTIPLY:type=OP_MULTIPLY;break;
                case I_DIVIDE:type=OP_DIVIDE;break;
            }
            if(type!=UNKNOWN)
            {
                tl->n++;//matc h "/ *"
                struct XTtree *now_tree=init_XTtree(2);
                now_tree->tree_type=type;
                XTlist_add(now_tree->child,struct XTtree *,tmp_root);
                XTlist_add(now_tree->child,struct XTtree *,do_exp_num(tl));
                tmp_root=now_tree;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    return tmp_root;
};

struct XTtree * do_exp_exp(struct token_list *tl)
{
    struct XTtree *start = do_exp_pri(tl);
    struct XTtree *tmp_root=start;
    while(1)//注意这里不知道{ op S}的重复次数
    {
        struct token *a_token_2 = token_list_get(tl,0,0);
        if(a_token_2!=NULL)
        {
            enum parser_node_type type=UNKNOWN;
            switch(a_token_2->type)
            {
                case I_ADD:type=OP_ADD;break;
                case I_REDUCE:type=OP_REDUCE;break;
            }
            if(type!=UNKNOWN)
            {
                tl->n++;//matc h "+ -"
                struct XTtree *now_tree=init_XTtree(0);
                now_tree->tree_type=type;
                XTlist_add(now_tree->child,struct XTtree *,tmp_root);
                XTlist_add(now_tree->child,struct XTtree *,do_exp_pri(tl));
                tmp_root=now_tree;
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }
    }
    return tmp_root;
};

struct XTtree * do_function(struct token_list *tl)
{
    struct token *a_token = token_list_get(tl,0,0);
    struct XTtree *root_tree=init_XTtree(0);
    root_tree->tree_type=FUNCTION_EXP;
    root_tree->content=a_token->is;
    tl->n++;//match function
    tl->n++;//match "("
    //匹配参数列表
    struct token *a_token_2 = token_list_get(tl,0,0);
    if(a_token_2!=NULL && a_token_2->type!=I_RIGHT_SMALLQ)
    {
        XTlist_add(root_tree->child,struct XTtree *,do_all_exp(tl));
    }
    a_token_2 = token_list_get(tl,0,0);
    while(a_token_2!=NULL && a_token_2->type==I_COMMA)
    {
        tl->n++;//match ","
        XTlist_add(root_tree->child,struct XTtree *,do_all_exp(tl));
        a_token_2 = token_list_get(tl,0,0);
    }
    tl->n++;//match ")"
    return root_tree;

};

struct XTtree * do_exp_num(struct token_list *tl)
 {
    struct token *a_token = token_list_get(tl,0,0);
    switch(a_token->type)
    {
    case I_NUMBER:
        {
            tl->n++;//match number
            struct XTtree *exp_tree=init_XTtree(0);
            exp_tree->content=a_token->is;
            exp_tree->tree_type=NUMBER;
            return exp_tree;
        }
        break;
    case I_LEFT_SMALLQ:
        {
            tl->n++;//match "("
            struct XTtree *tmp = do_exp_exp(tl);
            tl->n++;//match ")"
            return tmp;
        }
    case I_IDEN:
        {
            struct token *a_token_next = token_list_get(tl,1,0);
            if(a_token_next!=NULL && a_token_next->type==I_LEFT_SMALLQ)
            {
                return do_function(tl);
            }
            else
            {
                tl->n++;//match iden
                struct XTtree *exp_tree=init_XTtree(0);
                exp_tree->content=a_token->is;
                exp_tree->tree_type=IDEN;
                return exp_tree;
            }
        }
        break;
    default:;
    }
    return NULL;
}


struct XTtree * do_if(struct token_list *tl)
{
    struct XTtree * root = init_XTtree(2);
    root->tree_type=IF_STMT;
    tl->n+=1;//match if
    tl->n+=1;//match (
    struct XTtree * if_exp=do_bool_exp(tl);
    XTlist_add(root->child,struct XTtree *,if_exp);
    tl->n+=1;// match )
    XTlist_add(root->child,struct XTtree *,do_stmt_specific(tl));

    struct token *a_token = token_list_get(tl,1,0);
    if(a_token!=NULL && a_token->type==I_ELSE)
    {
        tl->n+=2;//match ";"&&"else"
        XTlist_add(root->child,struct XTtree *,do_stmt_specific(tl));
    }

    return root;
};

struct XTtree * do_while(struct token_list *tl)
{
    struct XTtree * root = init_XTtree(2);
    root->tree_type=WHILE_STMT;
    tl->n+=1;
    tl->n+=1;
    struct XTtree * while_exp=do_bool_exp(tl);
    XTlist_add(root->child,struct XTtree *,while_exp);
    tl->n+=1;
    XTlist_add(root->child,struct XTtree *,do_stmt_specific(tl));
    return root;
};

struct XTtree * do_stmt(struct token_list *tl)
{
    struct XTtree * start = do_stmt_specific(tl);
    struct XTtree * root = init_XTtree(0);
    XTlist_add(root->child,struct XTtree *,start);
    root->tree_type=STMT;
    while(1)
    {
        struct token *a_token_2 = token_list_get(tl,0,0);
        if(a_token_2==NULL)return root;
        switch(a_token_2->type)
        {
        case I_END_LINE_F:
            tl->n++;
            start=do_stmt_specific(tl);
            if(start!=NULL) XTlist_add(root->child,struct XTtree *,start);
            break;
        default:goto end;break;
        }
    }
    end:return root;
}

struct XTtree * do_let(struct token_list *tl)
{
    struct XTtree *root_tree = init_XTtree(0);
    root_tree->tree_type=LET_STMT;
    tl->n++;//match "let"
    XTlist_add(root_tree->child,struct XTtree *,do_exp_exp(tl));
    tl->n++;//match "="
    XTlist_add(root_tree->child,struct XTtree *,do_all_exp(tl));
    return root_tree;
};

struct XTtree * do_stmt_specific(struct token_list *tl)
{
    struct token *a_token_2 = token_list_get(tl,0,0);
    struct token *next_token = token_list_get(tl,1,0);
    if(a_token_2==NULL)return NULL;
    switch(a_token_2->type)
    {
        case I_NUMBER:
        case I_LEFT_SMALLQ:
            {
                struct XTtree *tmp=do_exp_exp(tl);
                return tmp;
            }
            break;
        case I_IDEN:
            {
                struct XTtree *tmp=NULL;
                switch(next_token->type)
                {
                case I_EQAUL:
                    tmp=do_assign_stmt(tl);
                    return tmp;
                    break;
                }
                tmp=do_exp_exp(tl);
                return tmp;
            }
            break;
        case I_LEFT_BIGQ:
            {
                tl->n++;
                struct XTtree *tmp=do_stmt(tl);
                tl->n++;
                return tmp;
            }
            break;
        case I_WHILE:
            {
                return do_while(tl);
            }
            break;
        case I_IF:
            {
                return do_if(tl);
            }
            break;
        case I_LET:
            {
                return do_let(tl);
            }
            break;
        default:
            return NULL;
            break;
    }
}
