#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"
#include "parser.h"
#include "../datastr/list.h"

char *lang_node_type_comment[]={
"stmt",
"exp",
"number",
"+",
"-",
"*",
"/",
};

struct XTtree *init_XTtree(int n)
{
    struct XTtree *t;
    t =(struct XTtree *)malloc(sizeof(struct XTtree));
    t->child=init_XTlist(n,sizeof(struct XTtree *));
    t->token_is="";
    t->token_type=I_UNDEF;
    t->child_num=n;
    return t;
}

//可视化的树状显示
void print_XTtree_V(struct XTtree *t,int where_i)
{
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
        for(int i=1;i<t->child->len;i++)
        {
            printf(",");
            print_XTtree(XTlist_get(t->child,i,struct XTtree *));
        }
        printf(")");
    }

}
//布尔表达式
struct XTtree * do_bool_exp(struct token_list *tl)
{
    struct XTtree *tmp_root=init_XTtree(2);
    //printf("#%s#",token_list_get(tl,0,0)->is);
    struct XTtree *tmp=do_exp_exp(tl);

    XTlist_add(tmp_root->child,struct XTtree *,tmp);
    struct token *a_token = token_list_get(tl,0,0);
   // printf("#%s-%p#",a_token->is,tmp);
    //printf("+%d+",tl->t[tl->n].type);

    switch(a_token->type)
    {
    case I_LEFT_K:
        tmp_root->token_type=a_token->type;
        break;
    case I_RIGHT_K:
        tmp_root->token_type=a_token->type;
        break;
    case I_SMALLER_EQAUL:
        tmp_root->token_type=a_token->type;
        break;
    case I_BIGER_EQAUL:
        tmp_root->token_type=a_token->type;
        break;
    default:break;
    }
    tl->n++;//match > < >= <=
    XTlist_add(tmp_root->child,struct XTtree *,do_exp_exp(tl));
    return tmp_root;

};
/**

//简单四则运算的parser测试

S -> fac { op S}
fac -> num | (S)

fac和do_exp_num对应
S和do_exp_exp对应
printf("=%d=",a_token_2->type);
*/
struct XTtree * do_exp_exp(struct token_list *tl)
{
    struct XTtree *start = do_exp_num(tl);
    struct XTtree *tmp_root=start;
    while(1)//注意这里不知道{ op S}的重复次数
    {
        struct token *a_token_2 = token_list_get(tl,0,0);
        //printf("/t:w:%d/",tl->n);
        if(a_token_2!=NULL &&
         (a_token_2->type==I_ADD
        || a_token_2->type==I_REDUCE
        || a_token_2->type==I_MULTIPLY
        || a_token_2->type==I_DIVIDE
        ))
        {
           // puts("??");
            tl->n++;//match "+ - / *"
         //   puts("n++");
            struct XTtree *now_tree=init_XTtree(2);
            now_tree->token_type=a_token_2->type;
            XTlist_add(now_tree->child,struct XTtree *,tmp_root);
            XTlist_add(now_tree->child,struct XTtree *,do_exp_num(tl));
           // now_tree->child[0]=tmp_root;
           // now_tree->child[1]=do_exp_num(tl);
            tmp_root=now_tree;
        }
        else
        {
            //tl->n++;
            break;
        }
    }
    //printf("/t:w:%d/",tl->n);
    return tmp_root;
};

struct XTtree * do_function(struct token_list *tl)
{
    struct token *a_token = token_list_get(tl,0,0);
    struct XTtree *root_tree=init_XTtree(0);
    root_tree->token_type=T_FUN;
    root_tree->token_is=a_token->is;
    tl->n++;//match function
    tl->n++;//match "("
    //匹配参数列表
    struct token *a_token_2 = token_list_get(tl,1,0);
    if(a_token_2!=NULL && a_token_2->type!=I_RIGHT_SMALLQ)
    {
        XTlist_add(root_tree->child,struct XTtree *,do_exp_exp(tl));
    }
    a_token_2 = token_list_get(tl,0,0);
    printf("=%s=2",a_token_2->is);
    while(a_token_2!=NULL && a_token_2->type==I_COMMA)
    {
        tl->n++;//match ","
        XTlist_add(root_tree->child,struct XTtree *,do_exp_exp(tl));
        a_token_2 = token_list_get(tl,0,0);
        printf("=%d=",a_token_2->type);
    }
    tl->n++;//match ")"
    return root_tree;

};

struct XTtree * do_exp_num(struct token_list *tl)
{
    struct token *a_token = token_list_get(tl,0,0);
    //判断a_token
    switch(a_token->type)
    {
    case I_NUMBER:
        {
            tl->n++;//match number
            struct XTtree *exp_tree=init_XTtree(0);
            exp_tree->token_type=I_NUMBER;
            exp_tree->token_is=a_token->is;
            //printf("-%d-",tl->t[tl->n].type);
            return exp_tree;
        }
        break;
    case I_LEFT_SMALLQ:
        {
            tl->n++;//match "("
            //puts("KK");
            struct XTtree *tmp = do_exp_exp(tl);
            tl->n++;//match ")"
            //puts("ww");
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
                exp_tree->token_type=I_IDEN;
                exp_tree->token_is=a_token->is;
                //printf("-%d-",tl->t[tl->n].type);
                return exp_tree;
            }
        }
        break;
    default:;
    }
    return NULL;
}

struct XTtree * do_while(struct token_list *tl)
{
    struct XTtree * root = init_XTtree(2);
    root->token_type=I_WHILE;
    root->token_is="while";
    tl->n+=1;
    tl->n+=1;
    struct XTtree * while_exp=do_bool_exp(tl);
    XTlist_add(root->child,struct XTtree *,while_exp);
    tl->n+=1;
   // printf("/%d/ ",tl->t[tl->n].type);
    XTlist_add(root->child,struct XTtree *,do_stmt_specific(tl));
    return root;
};

struct XTtree * do_stmt(struct token_list *tl)
{
    struct XTtree * start = do_stmt_specific(tl);
    struct XTtree * root = init_XTtree(0);
    XTlist_add(root->child,struct XTtree *,start);
    root->token_is="stmt";
    root->token_type=0;
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

struct XTtree * do_stmt_specific(struct token_list *tl)
{
    struct token *a_token_2 = token_list_get(tl,0,0);
    if(a_token_2==NULL)return NULL;
    switch(a_token_2->type)
    {
        case I_NUMBER:
            {
                struct XTtree *tmp=do_exp_exp(tl);
                return tmp;
            }
            break;
        case I_IDEN:
            {
                struct XTtree *tmp=do_exp_exp(tl);
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
        default:
            return NULL;
            break;
    }
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
    struct XTtree *s=do_stmt(&tl);
    puts("Tree:");
    print_XTtree_V(s,0);
    return 0;
}
