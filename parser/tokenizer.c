#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

#define NUM_KEYWORDS 3
#define I_IF 0
#define I_ELSE 1
#define I_WHILE 2
#define I_STRING 3
#define I_IDEN 4
#define I_NUMBER 5
#define I_LEFT_BIGQ 6
#define I_RIGHT_BIGQ 7
#define I_LEFT_SMALLQ 8
#define I_RIGHT_SMALLQ 9
#define I_RIGHT_K 10
#define I_LEFT_K 11
#define I_BIGER_EQAUL 12
#define I_SMALLER_EQAUL 13
#define I_NOT_EQAUL 14
#define I_END_LINE 15
#define I_ADD 16
#define I_REDUCE 17
#define I_MULTIPLY 18
#define I_DIVIDE 19
#define I_EQAUL 20
#define I_RIGHT_K_EQAUL 21
#define I_LEFT_K_EQAUL 22
#define I_EQAUL_VAR 23

char *type_print[100] = {
"if",
"else",
"while",
"string",
"iden",
"number",
"f: {",
"f: }",
"f: (",
"f: )",
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
};
char *dic[20] = {
"if",
"else",
"while",
};

struct tok_input{
char *start;
char *end;
char *buf;
};

struct token{
int type;
char *is;
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
    if(ch=='\n' || ch=='\t' || ch== ' ' || ch=='\r'){return 1;}
    else{return 0;}
}
//对比关键字
int which_keywords(char *s)
{
    for(int i=0;i<NUM_KEYWORDS;i++)
    {
        if(0==strcmp(s,dic[i]))
        {
            return i;
        }
    }
    return -1;
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
                for(;*s!='\n';s++)
                {
                    ;
                }
                continue;
            }
            else if(*s=='*')
            {
                for(++s;;++s)
                {
                    if(*s=='*' && *(s+1)=='/'){
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
            for(;*s!='\n';s++)
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
                case '=':t->type=I_EQAUL_VAR;break;
                default:t->type=I_EQAUL;s--;
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
                case '=':t->type=I_LEFT_K_EQAUL;break;
                default:t->type=I_LEFT_K;s--;
            }
            break;
        case '>':
            switch(*(s++))
            {
                case '=':t->type=I_RIGHT_K_EQAUL;break;
                default:t->type=I_RIGHT_K;s--;
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
        }
    }
    return s-s_start;
}

int main()
{
    FILE *so =fopen("tmp.xt","rb");
    char *string_in = file_into_string(so);
    struct token t;
    for(;;)
    {
        int a=get_next_token(string_in,&t);
        string_in+=a;
        if(a==0)break;
        else{
            print_token(&t);
        }
    }
    return 0;
}
