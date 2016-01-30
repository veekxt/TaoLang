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
    t->token_is="XTNULL";
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
    if(t==NULL)return;
    if(strlen(t->token_is)==0)
    {
        printf("#%d#",t->token_type);
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


struct XTtree * do_exp_exp(struct token_list *tl)
{
    printf("%s\n",tl->t->is);
    //struct token *a_token = token_list_get(tl,0,1);
    struct token *a_token_2 = token_list_get(tl,1,0);
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
        start_tree->child[0] = do_exp_num(tl);
        printf("left-ok\n");
        tl->n+=1;
        printf("###%s###\n",token_list_get(tl,0,0)->is);
        start_tree->child[1] = do_exp_exp(tl);
        printf("right-ok\n");
        return start_tree;
    }
    else{
        return do_exp_num(tl);
    }

}

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
            token_list_get(tl,0,1);///match ")"
            return tmp;
        }
        break;
    default:;
    }
}

struct XTtree * do_exp_old(struct token_list *tl)
//已弃用，无法处理括号
{
    struct XTtree *exp_tree=init_XTtree(2);
    struct token a_token=tl->t[tl->n];
    if(a_token.type==I_LEFT_SMALLQ)
    {
        /*
        tl->n++;
        struct XTtree *tmp = do_exp(tl);
        if(tl->t[tl->n++].type==I_RIGHT_SMALLQ)
        {
            return tmp;
        }
        */
    }
    else if(a_token.type==I_NUMBER)
    {
        if(tl->t[tl->n+1].type==I_ADD || tl->t[tl->n+1].type==I_REDUCE || tl->t[tl->n+1].type==I_MULTIPLY || tl->t[tl->n+1].type==I_DIVIDE)
        {
            struct XTtree *l=init_XTtree(2);
            l->token_type=I_NUMBER;
            l->token_is=a_token.is;
            exp_tree->token_type=tl->t[tl->n+1].type;
            exp_tree->token_is=tl->t[tl->n+1].is;
            exp_tree->child[0]=l;
            //printf("%d\n",tl->t[tl->n+1].type);
            tl->n+=2;
            exp_tree->child[1]=do_exp_old(tl);
            return exp_tree;
        }
        else
        {
            tl->n+=1;
            exp_tree->token_type=I_NUMBER;
            exp_tree->token_is=a_token.is;
            //note : free child
            return exp_tree;
        }
    }
    return exp_tree;
}

int main(void)
{
    struct token_list tl;
    file_to_token_to_array("tmp2.xt",&tl);
    for(int i=0; i<tl.max_len; i++)
    {
        print_token(&tl.t[i]);
    }
    struct XTtree *s=do_exp_exp(&tl);
    print_XTtree(s);
    //printf("%d-%p",s->child_num,s->child);
    return 0;
}
