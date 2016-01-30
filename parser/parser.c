#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"
#include "parser.h"

struct XTtree *init_XTtree(int n)
{
    struct XTtree *t;
    t =(struct XTtree *)malloc(sizeof(struct XTtree));
    if(n==0)t->child=NULL;
    else t->child=(struct XTtree **)malloc(sizeof(struct XTtree *)*n);
    t->token_is="";
    t->token_type=I_UNDEF;
    for(int i=0; i<n; i++)
    {
        t->child[i]=NULL;
    }
    t->child_num=n;
    return t;
}

void print_XTtree(struct XTtree *t)
{
    //输出一棵树，格式为
    //根（子节点1，子节点2）
    //例子 1/23-(1+90) ==> -(/(1,23),+(1,90))
    if(t==NULL)return;
    if(strlen(t->token_is)==0)
    {
        printf("%s",type_print[t->token_type]+3);
    }
    else
    {
        printf(t->token_is);
    }

    if(t->child!=NULL)
    {
        printf("(");
        print_XTtree(t->child[0]);
        for(int i=1;i<t->child_num;i++)
        {
            printf(",");
            print_XTtree(t->child[i]);
        }
        printf(")");
    }

}
//
struct XTtree * do_exp_exp(struct token_list *tl)
{
    struct XTtree *start = do_exp_num(tl);
    struct XTtree *tmp_root=start;
    while(1)
    {
        struct token *a_token_2 = token_list_get(tl,0,1);
        if(a_token_2!=NULL &&
         (a_token_2->type==I_ADD
        || a_token_2->type==I_REDUCE
        || a_token_2->type==I_MULTIPLY
        || a_token_2->type==I_DIVIDE
        ))
        {
            struct XTtree *now_tree=init_XTtree(2);
            now_tree->token_type=a_token_2->type;
            //now_tree->token_is="";
            now_tree->child[0]=tmp_root;
            now_tree->child[1]=do_exp_num(tl);
            tmp_root=now_tree;
        }
        else
        {
            break;
        }
    }
    return tmp_root;
};

struct XTtree * do_exp_num(struct token_list *tl)
{
    struct token *a_token = token_list_get(tl,0,1);
    switch(a_token->type)
    {
    case I_NUMBER:
        {
            struct XTtree *exp_tree=init_XTtree(0);
            exp_tree->token_type=I_NUMBER;
            exp_tree->token_is=a_token->is;
            return exp_tree;
        }
        break;
    case I_LEFT_SMALLQ:
        {
            struct XTtree *tmp = do_exp_exp(tl);
            //token_list_get(tl,0,1);///match ")"
            return tmp;
        }
        break;
    default:;
    }
}

/***

右结合，无优先级

S ::= fac
S ::= fac OP S
fac ::= NUM | '(' S ')'
NUM ::= 数字
OP ::= 加减乘除

//S
struct XTtree * do_exp_exp(struct token_list *tl)
{
    struct XTtree *start = do_exp_num(tl);
    struct token *a_token_2 = token_list_get(tl,0,1);
    if( a_token_2 !=NULL &&
        (a_token_2->type==I_ADD
        || a_token_2->type==I_REDUCE
        || a_token_2->type==I_MULTIPLY
        || a_token_2->type==I_DIVIDE
        )
       )
    {
        struct XTtree *start_tree=init_XTtree(2);
        start_tree->token_type=a_token_2->type;
        start_tree->token_is=a_token_2->is;
        start_tree->child[0] = start;
        start_tree->child[1] = do_exp_exp(tl);
        return start_tree;
    }
    else{
        return start;
    }

}

//fac
struct XTtree * do_exp_num(struct token_list *tl)
{
    struct token *a_token = token_list_get(tl,0,1);
    switch(a_token->type)
    {
    case I_NUMBER:
        {
            struct XTtree *exp_tree=init_XTtree(0);
            exp_tree->token_type=I_NUMBER;
            exp_tree->token_is=a_token->is;
            return exp_tree;
        }
        break;
    case I_LEFT_SMALLQ:
        {
            struct XTtree *tmp = do_exp_exp(tl);
            //token_list_get(tl,0,1);///match ")"
            return tmp;
        }
        break;
    default:;
    }
}

******/
int main(void)
{
    struct token_list tl;
    file_to_token_to_array("tmp2.xt",&tl);
    puts("Token Stream:");
    for(int i=0; i<tl.max_len; i++)
    {
        print_token(&tl.t[i]);
    }
    struct XTtree *s=do_exp_exp(&tl);
    puts("Tree:");
    print_XTtree(s);
    getchar();
    return 0;
}
