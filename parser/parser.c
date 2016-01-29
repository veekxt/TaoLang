#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"
#include "parser.h"

struct XTtree *init_XTtree(int n)
{
    struct XTtree *t;
    t =(struct XTtree *)malloc(sizeof(struct XTtree));
    t->child=(struct XTtree **)malloc(sizeof(struct XTtree *)*n);
    t->token_is="XTNULL";
    t->token_type=I_UNDEF;
    for(int i=0;i<n;i++)
    {
        t->child[i]=NULL;
    }
    t->child_num=n;
    return t;
}

void visit(struct XTtree *t)
{
    if(t==NULL)return;
    if(strlen(t->token_is)==0)
    {
        printf("#%d#",t->token_type);
    }else
    {
        printf(t->token_is);
    }
    printf("(");
    visit(t->child[0]);
    printf(",");
    visit(t->child[1]);
    printf(")");
}

struct XTtree * do_exp(struct token_list *tl)
{
    struct XTtree *exp_tree=init_XTtree(2);
    struct token a_token=tl->t[tl->n];
    if(a_token.type==I_LEFT_SMALLQ)
    {
        exp_tree->token_type=I_NUMBER;
        exp_tree->child=NULL;
        tl->n++;
        struct XTtree *tmp = do_exp(tl);
        if(tl->t[tl->n++].type==I_RIGHT_SMALLQ)
        {
            return tmp;
        }
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
            exp_tree->child[1]=do_exp(tl);
            return exp_tree;
        }
        else
        {
            exp_tree->token_type=I_NUMBER;
            exp_tree->token_is=a_token.is;
            //note : free child
            return exp_tree;
        }
    }
}

int main(void)
{
    struct token_list tl;
    file_to_token_to_array("tmp2.xt",&tl);
    for(int i=0;i<tl.max_len;i++)
    {
        print_token(&tl.t[i]);
    }
    struct XTtree *s=do_exp(&tl);
    visit(s);
    return 0;
}
