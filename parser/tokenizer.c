#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tokenizer.h"
/**
类型：
标量
    字符串
    数字
        整形
        浮点
    bool
关键字
运算符
界符
标识符
*/

#define is_identifier_start(c) (\
              (c >= 'a' && c <= 'z')\
               || (c >= 'A' && c <= 'Z')\
               || c == '_')

#define is_identifier_char(c) (\
              (c >= 'a' && c <= 'z')\
               || (c >= 'A' && c <= 'Z')\
               || (c >= '0' && c <= '9')\
               || c == '_')

char *type_print[100] =
{
    "if",
    "else",
    "while",
    "string",
    "iden",
    "number",
    "j: {",
    "j: }",
    "j: (",
    "j: )",
    "f: >",
    "f: <",
    "f: >=",
    "f: <=",
    "f: !=",
    "j: ;",
    "f: +",
    "f: -",
    "f: *",
    "f: /",
    "f: =",
    "f: >=",
    "f: <=",
    "f: ==",
    "j: ,",
    "def",
    "return",
    "un_def",
};

struct tok_input
{
    char *start;
    char *end;
    char *buf;
};

//获取文件长度
long int get_file_size(FILE *fp)
{
    fseek(fp,0,SEEK_END);
    long int size=ftell(fp);
    rewind(fp);
    return size;
}
//把文件读入字符串
char * file_into_string(FILE *fp)
{
    long int file_size=get_file_size(fp);
    char *s = (char *)malloc(1+sizeof(char)*file_size);
    s[file_size]=0;
    fread(s,1,file_size,fp);
    return s;
}

//判断字符是否是空白分隔符
int is_white(char ch)
{
    if(ch=='\n' || ch=='\t' || ch== ' ' || ch=='\r')
    {
        return 1;
    }
    else
    {
        return 0;
    }
}
//对比关键字
int which_keywords(char *s)
{
    if(0==strcmp(s,"if")) return I_IF;
    else if(0==strcmp(s,"else")) return I_ELSE;
    else if(0==strcmp(s,"while")) return I_WHILE;
    else if(0==strcmp(s,"def")) return I_DEF;
    else if(0==strcmp(s,"return")) return I_RETURN;
    else return -1;
}
void print_token(struct token *t)
{
    char *is_tmp=t->is;
    if(strlen(t->is)==0)
    {
        is_tmp=type_print[t->type]+3;
    }
    printf("%-20s %-20s\n",type_print[t->type],is_tmp);
}

//从s起始读取一个词素,返回读取的字符数
long int get_next_token(char *s,struct token *t)
{
    if(*s=='\0') return 0;
    int len_ch=0;
    int len_py=0;
    char *s_start = s;
    struct tok_input tok;
    t->is="";
    for(;;)
    {
        while(is_white(*s))
        {
            s++;
            len_py++;
        }
        if(*s=='/')
        {
            s++;
            if(*(s++)=='/')
            {
                for(; *s!='\n'; s++)
                {
                    ;
                }
                continue;
            }
            else if(*s=='*')
            {
                for(++s;; ++s)
                {
                    if(*s=='*' && *(s+1)=='/')
                    {
                        s=s+2;
                        break;
                    }
                }
                continue;
            }
            else
            {
                s--;
                break;
            }
        }
        else if(*s=='#')
        {
            for(; *s!='\n'; s++)
            {
                ;
            }
            continue;
        }
        else break;
    }
    if(*s=='\0') return 0;
    //字母开头
    tok.start=s;
    if(is_identifier_start(*s))
    {
        s++;
        len_ch++;
        //获取完整标识符或关键字
        while(is_identifier_char(*s))
        {
            len_ch++;
            s++;
        }
        char *tmp=malloc(sizeof(char)*len_ch+1);
        memcpy(tmp,tok.start,len_ch);
        *(tmp+len_ch)='\0';
        tok.buf=tmp;
        int which_keyword=which_keywords(tok.buf);
        if(which_keyword!=-1)
        {
            t->type=which_keyword;
            t->is=tmp;
        }
        else
        {
            t->type=I_IDEN;
            t->is=tmp;
        }
    }
    else if(*s>='0' && *s<='9')
    {
        s++;
        len_ch++;
        //获取完整标识符或关键字
        while(*s>='0' && *s<='9')
        {
            len_ch++;
            s++;
        }
        char *tmp=malloc(sizeof(char)*len_ch+1);
        memcpy(tmp,tok.start,len_ch);
        *(tmp+len_ch)='\0';
        tok.buf=tmp;

        t->type=I_NUMBER;
        t->is=tmp;

    }
    else
    {
        switch(*(s++))
        {
        case ';':
            t->type=I_END_LINE;
            break;
        case '+':
            t->type=I_ADD;
            break;
        case '*':
            t->type=I_MULTIPLY;
            break;
        case '-':
            t->type=I_REDUCE;
            break;
        case '/':
            t->type=I_DIVIDE;
            break;
        case '=':
            switch(*(s++))
            {
            case '=':
                t->type=I_EQAUL_VAR;
                break;
            default:
                t->type=I_EQAUL;
                s--;
            }
            break;
        case '(':
            t->type=I_LEFT_SMALLQ;
            break;
        case ')':
            t->type=I_RIGHT_SMALLQ;
            break;
        case '{':
            t->type=I_LEFT_BIGQ;
            break;
        case '}':
            t->type=I_RIGHT_BIGQ;
            break;
        case '<':
            switch(*(s++))
            {
            case '=':
                t->type=I_LEFT_K_EQAUL;
                break;
            default:
                t->type=I_LEFT_K;
                s--;
            }
            break;
        case '>':
            switch(*(s++))
            {
            case '=':
                t->type=I_RIGHT_K_EQAUL;
                break;
            default:
                t->type=I_RIGHT_K;
                s--;
            }
            break;

        case '"':
        {
            tok.start=s;

            while(!(*s=='"'&& !(*(s-1)=='\\')) || *s=='\0')
            {
                s++;
            }
            s++;
            int len_ch=s-tok.start;
            char *tmp=malloc(sizeof(char)*(len_ch));
            memcpy(tmp,tok.start,len_ch);
            *(tmp+(s-tok.start)-1)='\0';
            tok.buf=tmp;
            t->type=I_STRING;
            t->is=tmp;
            break;
        }
        case ',':
            t->type=I_COMMA;
            break;
        }
    }
    return s-s_start;
}
struct token *token_list_get(struct token_list *tl,int which,int excursion)
{
    struct token *tmp = tl->n+which < tl->max_len ? &tl->t[tl->n+which]:NULL;
    if(excursion>=0)tl->n+=excursion;
    return tmp;
};
int file_to_token_to_array(const char *file_name,struct token_list *tl)
{
    FILE *so =fopen(file_name,"rb");
    if(so==NULL)return -1;
    else
    {
        int init_len=10000;//词素数组初始大小
        tl->t=(struct token *)malloc(sizeof(struct token)*init_len);
        char *string_in = file_into_string(so);
        struct token tmp;
        int i;
        for(i=0;; i++)
        {
            int a=get_next_token(string_in,&tmp);
            string_in+=a;
            if(a==0)break;
            else
            {
                if(i>=init_len)
                {
                    tl->t = (struct token *)realloc(tl->t,sizeof(struct token)*(i+1));
                }
                tl->t[i]=tmp;

            }
        }
        tl->max_len=i;
        tl->n=0;
    }
    return 0;
}
