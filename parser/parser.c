#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"
#include "parser.h"
#include "../datastr/list.h"

struct XTtree *init_XTtree(int n)
{
    struct XTtree *t;
    t =(struct XTtree *)malloc(sizeof(struct XTtree));
    t->child=init_XTlist(n,sizeof(struct XTtree *));
    for(int i=0;i<n;i++)
    {
        XTlist_add(t->child,struct XTtree *,NULL);
        //XTlist_assign(t,i,struct XTtree *,NULL);
    }
    t->token_is="";
    t->token_type=I_UNDEF;
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

    if(t->child->data!=NULL)
    {
        printf("(");
        print_XTtree(XTlist_get(t->child,0,struct XTtree *));
        for(int i=1;i<t->child_num;i++)
        {
            printf(",");
            print_XTtree(XTlist_get(t->child,i,struct XTtree *));
        }
        printf(")");
    }

}
/**

//简单四则运算的parser测试

S -> fac { op S}
fac -> num | (S)

fac和do_exp_num对应
S和do_exp_exp对应

*/
struct XTtree * do_exp_exp(struct token_list *tl)
{
    struct XTtree *start = do_exp_num(tl);
    struct XTtree *tmp_root=start;
    while(1)//注意这里不知道{ op S}的重复次数
    {
        struct token *a_token_2 = token_list_get(tl,0,0);
        if(a_token_2!=NULL &&
         (a_token_2->type==I_ADD
        || a_token_2->type==I_REDUCE
        || a_token_2->type==I_MULTIPLY
        || a_token_2->type==I_DIVIDE
        ))
        {
            tl->n++;//match "+ - / *"
            struct XTtree *now_tree=init_XTtree(2);
            now_tree->token_type=a_token_2->type;
            XTlist_assign(now_tree->child,0,struct XTtree *,tmp_root);
            XTlist_assign(now_tree->child,1,struct XTtree *,do_exp_num(tl));
           // now_tree->child[0]=tmp_root;
           // now_tree->child[1]=do_exp_num(tl);
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
    struct token *a_token = token_list_get(tl,0,0);
    switch(a_token->type)
    {
    case I_NUMBER:
        {
            tl->n++;//match number
            struct XTtree *exp_tree=init_XTtree(0);
            exp_tree->token_type=I_NUMBER;
            exp_tree->token_is=a_token->is;
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
        break;
    default:;
    }
    return NULL;
}

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
